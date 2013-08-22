/* 
   This code has been released as public domain.
   Use at your own risc !
   
   Radim Kolar
   Fidonet 2:423/66.111

   Note: This program DON'T Notify PM (and WPS) about system reboot.
         'Reboot' PM-Hooks don't works with this program. (I hope)

   Rev. history
   
   29.6.95 sync.c created 
   2 .7.95 better error handling
   6. 7.95 added -halt and -reboot command line options
           help added
   8. 7.95 added error code 1 and exit when parameter is wrong.
           added count arguments check
   5. 6.96 added -wait sec, option. Can be used with -reboot or -halt
*/

#ifdef DEBUG
#define SHUTDOWNVALUE 1
#else
#define SHUTDOWNVALUE 0
#endif

#define INCL_BASE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <os2.h>

void Help()
{
 printf("\nsync [-halt | -reboot] [ -wait n]\n");
 exit(1);
}

int main(int argc, char *args[])
{
 int i;
 APIRET rc;
 int action=0; /* 0=sync, 1=halt , 2=reboot */
 HFILE  hf=0;
 ULONG dummy;
 ULONG waittime=0; /* 0 sec wait */

/* if(argc>3) Help();*/
 for(i=1;i<argc;i++)
  if(!strcmp(args[i],"-halt")) action=1;
  else if(!strcmp(args[i],"-reboot")) action=2;
  else if(!strcmp(args[i],"-wait")) waittime=1000*atoi(args[(i++)+1]);
  else { printf("\nUnknown switch %s",args[i]);Help();}

  rc = DosOpen("DOS$", &hf, &dummy, 0L, FILE_NORMAL, FILE_OPEN,
       OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE |
       OPEN_FLAGS_FAIL_ON_ERROR, NULL);

 switch(action)
 {
 case 0:  rc=DosShutdown(1);break;
 default: 
        if (waittime) printf("\nSystem will be halted/rebooted in %ld sec. Press CTRL-C to abort.\n",waittime/1000);
        DosSleep(waittime);
/*      if (_read_kbd(0,0,0)!=-1) { printf("\nAction Aborted by user request."); exit(2);}; */
        printf("\nPlease wait, shutdown still in progress.\n"
                 "DO NOT TURN POWER OFF NOW!\n"); 
        rc=DosShutdown(SHUTDOWNVALUE); 
        if(!rc) printf("\nShutdown has completed.\nIt is now safe to turn"
         " off your computer, or restart system \nby pressing Ctrl+Alt+Del.\a");
 }
 if(rc==87) printf("\nYou must have at least OS/2 version 2.1 to run this"
  " program.\n");
 if(rc==274) printf("\nsync: Filesystem is allready shutdown"
  " or shutdown is in progress.\n");
 if(action==2)
 {
     if(hf)
     {
        printf("\n\nRebooting system in 10 secs...");
        DosSleep(10000);
        /* 32-Bit reboot */
        DosDevIOCtl( hf, 0xd5, 0xab, NULL, 0, NULL, NULL, 0, NULL );

        /* old 16-bit reboot
        DosDevIOCtl(NULL, NULL, 0xab, 0xd5, hf);
        */
        DosClose(hf);
     }
     else
       printf ("\nDOS.SYS not installed. Can't Reboot.");
 }

 return rc;
}