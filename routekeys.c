#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define mprintf(args...) fprintf(stderr, args)

void   mysys(char *cmd) { char allcmd[512]; snprintf(allcmd, 512, "%s &>/dev/null &", cmd); system(allcmd); }
struct timeval64 { __u64 tv_sec, tv_usec; };
struct event64   { struct timeval64 time; __u16 type, code; __s32 value; };

//special key-sequence:
#define escapelen 4
char escapekeys[escapelen]={KEY_LEFTCTRL, KEY_LEFTALT, KEY_LEFTSHIFT, KEY_ESC}; //Ctrl-Alt-Shift-Esc
char escapevals[escapelen]={ 0,  0,  0,  0};
int checkEscapeSequence(struct input_event *ev)
{  int i, all;
   for (i=0, all=0; i<escapelen; i++) 
   {  if (escapekeys[i] ==ev->code) escapevals[i] =ev->value;
      if (escapevals[i]) all++;
   }  
   return all ==escapelen;
}

//main keyboard functions:
    struct event64         tmp; int tmpsize =sizeof(struct event64); 
                                int evsize  =sizeof(struct input_event); 
    struct input_event     *ev =(void*)&tmp+tmpsize-evsize;
    
int loopKeyboardINP()
{  struct uinput_user_dev uidev;
   int i, evbits[] ={ EV_SYN, EV_KEY, EV_MSC };
   int fdo =open("/dev/uinput", O_WRONLY | O_NONBLOCK);           if (fdo < 0)   return 1;
   for (i=0;i<sizeof(evbits);i++) if (ioctl(fdo, UI_SET_EVBIT, evbits[i]) < 0) { close(fdo); return 20+i; }
   for (i=0;i<KEY_MAX;       ++i) if (ioctl(fdo, UI_SET_KEYBIT, i)        < 0) { close(fdo); return 3; }

   memset(&uidev, 0, sizeof(uidev));
   snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
   uidev.id.bustype = BUS_USB;          uidev.id.vendor  = 0x1;
   uidev.id.version = 1;                uidev.id.product = 0x1;
   if (write(fdo, &uidev, sizeof(uidev)) < 0) { close(fdo); return 4; }
   if (ioctl(fdo, UI_DEV_CREATE)         < 0) { close(fdo); return 5; }

   mysys("fgnotify 'KEYS ROUTED HERE !'");

   while (1)                                   //from stdin - 64bit sized structs comming
   {  if (read (STDIN_FILENO, &tmp, tmpsize) < 0) { close(fdo); return 6; } 
      //mprintf("\r in: type: %d, code: %d, val: %d\n", ev->type, ev->code, ev->value); 
      ev->time.tv_sec  =ev->time.tv_usec =0;
      if (write(fdo, ev, evsize) < 0) { close(fdo); return 7; } //to device send arch-specific 32 or 64bit
      if (checkEscapeSequence(ev))    { mprintf(" in: Escape-Sequence detected, terminating\n"); 
                                        close(fdo); return 8; } 
   }
   mprintf("in: exiting\n");
   if (ioctl(fdo, UI_DEV_DESTROY) < 0) { close(fdo); return 9; }

   close(fdo); close(STDIN_FILENO); return 0;
}
int keyboardOUT(char *devname)
{  int pressterminate=0,
       fdi = open(devname, O_RDONLY);  if (fdi  <0) return 1;
   if (ioctl(fdi, EVIOCGRAB, 1) < 0)  { close(fdi); return 2; }

   while(1)                                    //from device events could be 32 or 64 bit long
   {  if (read (fdi, ev, evsize) < 0) { close(fdi); return 3; }
      //mprintf("\rout: type: %d, code: %d, val: %d\n", ev->type, ev->code, ev->value); 

      if (pressterminate && (ev->type ==1) && (ev->value >0)) { close(fdi); return ev->code; }

      tmp.time.tv_sec  =tmm.time.tv_usec =0;
      if (!pressterminate) 
         if (write(STDOUT_FILENO, &tmp, tmpsize) < 0) die("error: write"); //to stdout send 64bit 

      if (checkEscapeSequence(ev)) 
      { mprintf("out: Escape-Sequence detected, terminating on first-next-key-PRESS\n"); 
                                     mysys("fgnotify 'select dest'"); pressterminate =1; 
      } 
   }
   mprintf("out: exiting\n");
   close(fdi); close(STDOUT_FILENO); return 0;
}
//main function: //////////////////////////////////////////////////////////////////////////
#define die(args...)     if (1) { mprintf(args); goto __end; }
int main(int argc, char* argv[])
{   int                    fdo, fdi;
    int                    i, all;
    int                    err =0, retval =0, pressterminate =0;


    if      (argc==2 && !strcmp(argv[1], "inp")) retval =loopKeyboardINP(); 
    else if (argc==3 && !strcmp(argv[1], "out")) retval =loopKeyboardOUT(argv[2]); 
    else 
    {  mprintf("usage: 1) routekeys in\n"
               "          #... receiving stdin to new virtual keyboard HERE, on this comp\n"
               "       2) routekeys out /dev/input/devicename\n"
               "          #... blocking, reading device and sending data to stdout\n"
               "\n... btw, size of inputevent here is: %d, and timeval(2xlong): %d\n", 
               sizeof(struct input_event), sizeof(struct timeval)); 
       return 0;
    }
    mprintf("%s: exiting with value: %d\n", argv[1], retval);
    return retval;
}
