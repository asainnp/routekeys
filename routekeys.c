#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define mprintf(args...) fprintf(stderr, args)
#define die(args...)     if (1) { mprintf(args); goto __end; }

struct timeval64 { __u64 tv_sec, tv_usec; };
struct event64   { struct timeval64 time; __u16 type, code; __s32 value; };

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

int main(int argc, char* argv[])
{
    int                    fdo, fdi;
    struct uinput_user_dev uidev;
    struct input_event     *ev;
    int                    i, all;
    struct event64         tmp;
    int                    evsize  =sizeof(struct input_event); 
    int                    tmpsize =sizeof(struct event64); 
    int                    err =0, retval =0, pressterminate =0;

    ev =(void*)&tmp+tmpsize-evsize;

    if (argc==2 && !strcmp(argv[1], "inp")) //receiving stdin and make it as local keyboard************************
    {  fdo = open("/dev/uinput", O_WRONLY | O_NONBLOCK); if (fdo < 0) die("error: open /dev/uinput");

       if (ioctl(fdo, UI_SET_EVBIT, EV_SYN) < 0) die("error: ioctl");
       if (ioctl(fdo, UI_SET_EVBIT, EV_KEY) < 0) die("error: ioctl");
       if (ioctl(fdo, UI_SET_EVBIT, EV_MSC) < 0) die("error: ioctl");

       for (i = 0; i < KEY_MAX; ++i)
          if (ioctl(fdo, UI_SET_KEYBIT, i)  < 0) die("error: ioctl");

       memset(&uidev, 0, sizeof(uidev));
       snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
       uidev.id.bustype = BUS_USB;
       uidev.id.vendor  = 0x1;
       uidev.id.product = 0x1;
       uidev.id.version = 1;

       if (write(fdo, &uidev, sizeof(uidev)) < 0) die("error: write");
       if (ioctl(fdo, UI_DEV_CREATE)         < 0) die("error: ioctl");
       system("fgnotify 'KEYS ROUTED HERE !'");
       while(1)
       {  if (read (STDIN_FILENO, &tmp, tmpsize) < 0) die("error: read"); //from stdin must come 64bit sized structs
          mprintf(" in: type: %d, code: %d, val: %d\n", ev->type, ev->code, ev->value); 
          ev->time.tv_sec  = 0;
          ev->time.tv_usec = 0;
          if (write(fdo, ev, evsize) < 0) die("error: write"); //to device send arch-specific 32 or 64bit
          if (checkEscapeSequence(ev)) { mprintf(" in: Escape-Sequence detected, terminating\n"); break; } //all 4 keys are pressed
       }
       mprintf("in: exiting\n");
       if (ioctl(fdo, UI_DEV_DESTROY) < 0) die("error: ioctl");

       close(fdo); close(STDIN_FILENO); die("ok");
    }
    else if (argc==3 && !strcmp(argv[1], "out")) //sending local keyboard to stdout***********************************
    {                                                    if (argc <3) die("err%d: param 2 missing", err=1);
       fdi = open(argv[2], O_RDONLY);                    if (fdi  <0) die("error: failed opening %s\n", argv[2]);
       if (ioctl(fdi, EVIOCGRAB, 1) < 0) die("error: ioctl");

       while(1)
       {  mprintf("out:reading\n");
          if (read (fdi, ev, evsize) < 0) die("error: read"); //from device events could be 32 or 64 bit long
          mprintf("out:reading end.\n");
          mprintf("out: type: %d, code: %d, val: %d\n", ev->type, ev->code, ev->value); 

          if (pressterminate && (ev->type ==1) && (ev->value >0)) { retval =ev->code; break; }

          tmp.time.tv_sec  = 0;
          tmp.time.tv_usec = 0;
          if (!pressterminate) 
          if (write(STDOUT_FILENO, &tmp, tmpsize) < 0) die("error: write"); //to stdout send 64bit 

          if (checkEscapeSequence(ev)) { mprintf("out: Escape-Sequence detected, terminating on first-next-key-PRESS\n"); 
                                         pressterminate =1; } 
       }
       mprintf("out: exiting\n");
       close(fdi); close(STDOUT_FILENO); die("ok");
    }
    else err =1; //params not ok

__end:
    switch (err)
    {  case 1: mprintf("usage: 1) routekeys in\n"
                       "          #... receiving stdin to new virtual keyboard HERE, on this comp\n"
                       "       2) routekeys out /dev/input/devicename\n"
                       "          #... blocking, reading device and sending data to stdout\n"
                       "\n... btw, size of inputevent here is: %d, and timeval(2xlong): %d\n", sizeof(struct input_event), sizeof(struct timeval));
               break;
    }
    mprintf(" end.\n");
    //exit(retval);
    return retval;
}
