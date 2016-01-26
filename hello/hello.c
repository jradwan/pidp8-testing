// hello.c
// Hello World! LED test/learning experiment for PiDP-8

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "../ledcontrol.h"  // custom LED control functions
#include "../pdpcontrol.h"  // other PiDP-8 control functions


// control-c handling for infinite loop
static volatile int keepRunning = 1;
void intHandler(int dummy) {
   keepRunning = 0;
}


int main( int argc, char *argv[] )
{
   int j;

   start_multiplexer();

   signal(SIGINT, intHandler);

   printf("\nHello World!\n\nPress Ctrl-C to stop ...\n");

   while (keepRunning)  // infinite loop, wait for Ctrl-C
   {

      // start with all LEDs on
      leds_on();
      sleep ( 1 );
      leds_off();

      // turn on columns 0 and 11 for all rows
      for ( j = 0; j <= 4; j++ )
      {
         ledstatus[j] |= 1<<0;  
         ledstatus[j] |= 1<<11; 
      }

      // turn on rows 0 and 4
      ledstatus[0] = 07777;
      ledstatus[4] = 07777;

      sleep( 1 );
      leds_off();

      // turn on columns 1 and 10 for rows 1 and 3
      for ( j = 1; j <= 3; j++ ) {
         ledstatus[j] |= 1<<1;
         ledstatus[j] |= 1<<10;
      }

      // turn on rows 1 and 3
      ledstatus[1] = 03776;
      ledstatus[3] = 03776;

      sleep ( 1 );
      leds_off();

      // turn on middle of row 2
      ledstatus[2] = 01774;
      sleep ( 1 );
      leds_off();

      // try the scanner function
      leds_scan();

   } // end of infinite loop

   printf("\n\nDone!\n");
}
