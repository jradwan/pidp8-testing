// matrix.c
// LED-Matrix: menu driven LED test utility for PiDP-8

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "../ledcontrol.h"  // custom LED control functions
#include "../pdpcontrol.h"  // other PiDP-8 control functions

// sample paths of LEDs used for animation ----------------------------

// each LED in sequence, starting at top left, ending top right
uint8 chasePath[] = { 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x11, 0x12, 0x13, 0x14, 0x15,
                      0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x2c, 0x2b, 0x2a, 0x29,
                      0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x31, 0x32, 0x33,
                      0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x4c, 0x4b,
                      0x4a, 0x49, 0x48, 0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 0x87,
                      0x76, 0x77, 0x78, 0x79, 0x7a, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
                      0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x68, 0x67, 0x66, 0x65, 0x64,
                      0x63, 0x62, 0x61, 0x69, 0x6a, 0x6b, 0x6c, 0x71, 0x72, 0x75, 0x74,
                      0x73 };

// around the square
uint8 myPath[] = { 0x11, 0x21, 0x31, 0x41, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c,
                   0x4c, 0x3c, 0x2c, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11 };

uint8 anotherPath[] = { DF1, DF2, DF3, IF1, IF2, IF3, ION, PAUSE, RUN, IF3, IF2, IF1, DF3, DF2, DF1 };



int main( int argc, char *argv[] )
{
   char ledLoc[2], menuItem[1] = "1";
   char ledPos[4];
   int paused = 0;

   printf("\nWelcome to LED-Matrix\n");
   start_multiplexer();

   // loop the menu until the user quits
   while ( atoi(menuItem) != 0 ) {
      printf("\n");
      printf("1. Turn ON all LEDs\n");
      printf("2. Turn OFF all LEDs\n");
      printf("3. Single LED test\n");
      printf("4. LED animate pattern\n");
      printf("5. LED animate pattern (stepped)\n");
      printf("6. LED draw pattern\n");
      printf("0. Quit\n");
      printf("\n");
      printf("Selection: ");

      fgets(menuItem, 2, stdin);
      //printf("\nSelected %d\n", atoi(menuItem));
      switch( atoi(menuItem) )
      {
         case 1:
            printf("\nTurning ON all LEDs ...\n");
            leds_on();
            (void)getchar();
            break;
         case 2:
            printf("\nTurning OFF all LEDs ...\n");
            leds_off();
            (void)getchar();
            break;
         case 3:
            printf("\nInitiating single LED test ...\n");
            (void)getchar();
            leds_single_test();
            break;
         case 4:
            printf("\nInitiating LED animation ...\n");
            paused = 0;
            leds_animate( chasePath, sizeof(chasePath), paused );
            leds_animate( myPath, sizeof(myPath), paused );
            leds_animate( anotherPath, sizeof(anotherPath), paused );
            (void)getchar();
            break;
         case 5:
            printf("\nInitiating LED animation (stepped) ...\n");
            (void)getchar();
            paused = 1;
            leds_animate( chasePath, sizeof(chasePath), paused );
            leds_animate( myPath, sizeof(myPath), paused );
            leds_animate( anotherPath, sizeof(anotherPath), paused );
            break;
         case 6:
            printf("\nInitiating LED drawing ...\n");
            paused = 0;
            leds_draw( chasePath, sizeof(chasePath), paused );
            //leds_draw( myPath, sizeof(myPath), paused );
            //leds_draw( anotherPath, sizeof(anotherPath), paused );
            (void)getchar();
            break;
         case 0:
            break;
         default:
            printf("\n\nINVALID SELECTION [%s] ... Please try again\n", menuItem);
      } 
      //printf("\nback from function call\n");

   } // end of menu loop

   leds_off();
   printf("\n\nThanks for using LED-Matrix!\n\n");
}
