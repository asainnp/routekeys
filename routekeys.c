#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <pthread.h>
#include <signal.h>

char inout[]="none";
#define mprintf(args...)  //{ fprintf(stderr,"%s: ",inout); fprintf(stderr, args); }
#define mprintf2(args...)   { fprintf(stderr,"%s: ",inout); fprintf(stderr, args); }
int  globalQUIT =0;

void   mysys(char *cmd) 
{ char allcmd[512]; snprintf(allcmd, 512, "%s &>/dev/null &", cmd); system(allcmd); }

struct timeval64 { __u64 tv_sec, tv_usec; };
struct event64   { struct timeval64 time; __u16 type, code; __s32 value; };

//special key-sequence:
#define escapelen 4
char escapekeys[escapelen]={KEY_LEFTCTRL, KEY_LEFTALT, KEY_RIGHTALT,  KEY_RIGHTCTRL}; 
char escapevals[escapelen]={0,            0,           0,             0            };
int glbAll =0;
int checkEscapeSequence(struct input_event *iev)
{  int i, all;    if (iev->type != EV_KEY) return 0; //check keys only
   for (i=0, all=0; i<escapelen; i++) 
   {  if (escapekeys[i] ==iev->code) escapevals[i] =iev->value;
      if (escapevals[i]) all++;
   }  
   glbAll=all;
   return all==escapelen;
}

int createUIDEV(int *fd)
{  int i; struct uinput_user_dev uidev;
   #define myEVMAX 4
   int evbits[myEVMAX] ={ EV_SYN, EV_KEY, EV_MSC, EV_REP }; 

   *fd =open("/dev/uinput", O_WRONLY | O_NONBLOCK);        if (*fd < 0) return 1;

   for (i=0;i<myEVMAX;++i) if (ioctl(*fd, UI_SET_EVBIT, evbits[i]) < 0) return 20+i;
   //turning on all Keys, including all-kbd-keys and all possible mouse-buttons:
   for (i=0;i<KEY_MAX;++i) if (ioctl(*fd, UI_SET_KEYBIT, i)        < 0) return 5; 

   memset(&uidev, 0, sizeof(uidev)); 
   snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinputroutekeys"); 
   uidev.id.bustype = BUS_USB;            uidev.id.vendor  = 0x1; 
   uidev.id.version = 1;                  uidev.id.product = 0x1;

   if (write(*fd, &uidev, sizeof(uidev)) < 0) return 6;
   if (ioctl(*fd, UI_DEV_CREATE)         < 0) return 7;
   return 0;
}

//global vars:
    struct event64         evtmp; const int tmpsize =sizeof(struct event64); 
                                  const int evsize  =sizeof(struct input_event); 
    struct input_event     *ev =(void*)&evtmp +sizeof(struct event64)
                                              -sizeof(struct input_event);

static int prevPress=0; 
int writeEvent(int fdo, struct input_event *wev);

int writeEvent(int fdo, struct input_event *wev)
{  wev->time.tv_sec =wev->time.tv_usec =0;  
   if (write(fdo, wev, evsize)  < 0) return 1;
   return 0;
}

int signalEND =0;
//TWO main keyboard functions:
#define mreturn(rv) { close(fdo); close(STDIN_FILENO); return rv; }
int loopKeyboardINP()
{  
   int i,r, pressterminate=0, fdo =-1;
   r =createUIDEV(&fdo);                                      if (r>0) mreturn(100+r);   //uidev errors 1xx 
   mysys("fgnotify 'KEYS ROUTED HERE !(v1.2)'");

   while (!globalQUIT)                           //from stdin - 64bit sized structs comming
   {  r =read (STDIN_FILENO, &evtmp, tmpsize);                if (r<1) mreturn(11);    
      mprintf("\rtype: %d, code: %d, val: %d, all=%d\n", 
               ev->type,        ev->code, ev->value, glbAll); 
      if (pressterminate) if (ev->type ==EV_KEY && ev->value >0)       mreturn(1000+ev->code); //return KEY

      r =writeEvent(fdo, ev);                                 if (r>0) mreturn(12);
      if (checkEscapeSequence(ev)) { mprintf2("Escape-Sequence detected, terminating on next-PRESS\n"); 
                                     pressterminate=1; }
   }
   if (ioctl(fdo, UI_DEV_DESTROY) < 0)            mreturn(3);

   mreturn(signalEND?1:0);
}
#undef  mreturn
#define mreturn(rv) { close(fdi); close(STDOUT_FILENO); return rv; }
int loopDeviceOUT(char *devname)
{  int pressterminate=0, retval=0; fd_set fds; struct timeval tv;
   int fdi = open(devname, O_RDONLY);  if (fdi  <0) mreturn(1);
   if (ioctl(fdi, EVIOCGRAB, 1) < 0)                mreturn(2);
  
    
   while (!globalQUIT)               //from device events could be 32 or 64 bit long
   {  FD_ZERO(&fds); FD_SET(fdi, &fds); tv.tv_sec =0; tv.tv_usec =50000; //50ms
      if (-1 ==select(fdi+1,&fds,NULL,NULL, &tv))   mreturn(3);
      if (!FD_ISSET(fdi, &fds)) continue;
      if (read (fdi, ev, evsize) < 0)               mreturn(4);
      mprintf("\rout: type: %d, code: %d, val: %d, all=%d\n", evtmp.type, evtmp.code, evtmp.value, glbAll); 

      evtmp.time.tv_sec =evtmp.time.tv_usec =0;
      if (write(STDOUT_FILENO, &evtmp, tmpsize)   <0) mreturn(5); //to stdout send 64bit 
      if (pressterminate && (ev->type ==1) && (ev->value >0))
         if (ev->code==111) { mprintf2("Del after Escape-Sequence, terminating");  mreturn(6); } 
         else { pressterminate=0; mprintf2("continuing...\n"); }
      if (checkEscapeSequence(ev)) { mprintf2("Escape-Sequence detected\n"); pressterminate=1; }
   }
   mreturn(signalEND?1:0);
}
#undef  mreturn
void* loopDeviceOUTstart(void *arg) 
{  void *retval; 
   mprintf("thread starting, (dev=%s)\n", arg);
   *((int*)&retval) =loopDeviceOUT((char*)arg); 
   globalQUIT =1; // 1 down -> all down
   mprintf2("thread exiting, retval==%d (dev=%s)\n", retval, arg);
   return retval;
}
#define setSigHandler(sig) if (signal(sig,  sig_handler) == SIG_ERR) mprintf("err: can not catch signal "#sig"\n");
void sig_handler(int sig)
{   switch (sig) { case SIGINT: break;
                   case SIGSTOP: case SIGKILL: globalQUIT =signalEND =1; break; }
    mprintf2("signal %d catched\n", sig);
}
//main function: //////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{   int retval =0, outretval =0, pressterminate =0, i;
    ev =(void*)&evtmp +tmpsize-evsize;
    mprintf("main: evtmp=%016X, tmpsize=%d, ev=%016X, evsize=%d\n", &evtmp, tmpsize, ev, evsize);
    setSigHandler(SIGINT); setSigHandler(SIGSTOP); setSigHandler(SIGKILL);

    pthread_t tid[8]; //8 devices max (standard usage 1, keyb)
    int       retvals[8] ={0,0,0,0,0,0,0,0};

    if      (argc==2 && !strcmp(argv[1], "inp")) 
    {  sprintf(inout, "inp");
       retval =loopKeyboardINP();         //LOOP !!! 
       mprintf("exiting, org.retval==%d\n", retval);
       return (retval>1000) ?retval-1000 :0;         // INP-return:  =0 no key selected
                                                     //              >0 key-code returning
    }
    else if (argc >2 && !strcmp(argv[1], "out")) 
    {  sprintf(inout, "out");
       if (argc>7+2) { mprintf("err-max 8 devices allowed"); return 1; }
       for (i=2;i<argc;++i)
          if (0!=pthread_create(&tid[i-2], NULL, &loopDeviceOUTstart, argv[i])) //LOOP !!!
          {  mprintf("err-cannot create thread %d for device %s.", i-2, argv[i]); return 2;  }
       for (i=2;i<argc;++i) 
       {  pthread_join(tid[i-2], (void**)&retval);
          mprintf("thread %d exited with retval=%d\n", i-2, retval);
          retvals[i-2] =retval;  if (outretval <retval) outretval =retval;
       }
       return outretval;                            // OUT-return:   greatest err
       //OUT return value is not important to scripts that calling it (but above INP val is !!).
    }
    else 
    {  mprintf("usage: 1) routekeys inp\n"
               "          #... receiving stdin to new virtual keyboard HERE, on this comp\n"
               "       2) routekeys out /dev/input/devicename\n"
               "          #... blocking, reading device and sending data to stdout\n"
               "\n... btw, size of inputevent here is: %d, and timeval(2xlong): %d\n", 
               sizeof(struct input_event), sizeof(struct timeval)); 
    }  
    return 0; 
}
