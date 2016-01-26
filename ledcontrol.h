// ledcontrol.h
// LED control functions for the PiDP-8 front panel

#include <signal.h>
#include <unistd.h>
#include "ledlocations.h"       // LED locations in hex (constants)

typedef unsigned int    uint32;
typedef unsigned char   uint8;

extern uint32 ledstatus[8];     // bitfields: 8 ledrows of up to 12 LEDs


// turn off all LEDs on the front panel
void leds_off ( )
{
   int offrow;
   for( offrow = 0 ; offrow < 8; ++offrow )
      ledstatus[offrow] = 0;

}


// turn on all LEDs on the front panel
void leds_on ( )
{
   int onrow;
   for( onrow = 0; onrow < 8; ++onrow )
      ledstatus[onrow] = 07777;
}


// animate LEDs in pre-determined pattern
int leds_animate ( uint8 *animationPath, int pathSize, int paused )
{
   int arrayIdx = 0, ledRow = 0, delay = 0;

   // clear panel first
   leds_off();

   if ( paused )
      printf("\nSpecified pattern includes %i steps.\n", pathSize);

   // animate the specified pattern (one LED at a time)
   for ( ; ; )
   {
      if( delay++ >= 30 ) // change code so there's no delay in animation when "paused"
      {
         delay = 0;
         ledstatus[ledRow] = 0;
         ledstatus[ledRow=(animationPath[arrayIdx]>>4) - 1] = 04000 >> ((animationPath[arrayIdx]&0x0F) - 1);
         // if "paused" mode, stop and wait for user input between each LED
         if ( paused && arrayIdx < pathSize) 
         {
             if ( arrayIdx+1 < pathSize )
                 printf("Step %i: illuminating LED at position %o (Press ENTER to continue)", arrayIdx+1, animationPath[arrayIdx]);
             else
                 printf("Step %i: illuminating LED at position %o\nDone! Press ENTER to return to main menu ", arrayIdx+1, animationPath[arrayIdx]);
             (void)getchar();
         }
         if( ++arrayIdx > pathSize/sizeof(animationPath[0]) ) break;
       }

       usleep( 1000 );
   }
}


// draw LED pattern (leaving a lit trail)
int leds_draw ( uint8 *animationPath, int pathSize, int paused )
{
   int arrayIdx = 0, ledRow = 0, delay = 0;

   // clear panel first
   leds_off();

   if ( paused )
      printf("\nSpecified pattern includes %i steps.\n", pathSize);

   // draw the specified pattern (one LED at a time, leaving it on as we go)
   for ( ; ; )
   {
      if( delay++ >= 30 ) // change code so there's no delay in animation when "paused"
      {
         delay = 0;
         //ledstatus[ledRow] = 0;
         //                                                   octal controls
         //                                                   how many leds to
         //                                                   turn on
         //ledstatus[ledRow=(animationPath[arrayIdx]>>4) - 1] = 04000 >> ((animationPath[arrayIdx]&0x0F) - 1);
         ledstatus[ledRow=(animationPath[arrayIdx]>>7) - 1] = 04000 >> ((animationPath[arrayIdx]&0x0F) - 1);
         // if "paused" mode, stop and wait for user input between each LED
         if ( paused && arrayIdx < pathSize)
         {
             if ( arrayIdx+1 < pathSize )
                 printf("Step %i: illuminating LED at position %o (Press ENTER to continue)", arrayIdx+1, animationPath[arrayIdx]);
             else
                 printf("Step %i: illuminating LED at position %o\nDone! Press ENTER to return to main menu ", arrayIdx+1, animationPath[arrayIdx]);
             (void)getchar();
         }
         if( ++arrayIdx > pathSize/sizeof(animationPath[0]) ) break;
       }

       usleep( 1000 );
   }
}



// animate each LED, in order, printing the label, for testing purposes
void leds_single_test ( )
{
   int arrayIdx = 0, ledRow = 0;

   // path of LEDs used for animation
   uint8 ledTestArray[] = { DF1,DF2,DF3,IF1,IF2,IF3,PC01,PC02,PC03,PC04,PC05,PC06,PC07,PC08,PC09,PC10,PC11,PC12,
                            MA01,MA02,MA03,MA04,MA05,MA06,MA07,MA08,MA09,MA10,MA11,MA12,
                            MB01,MB02,MB03,MB04,MB05,MB06,MB07,MB08,MB09,MB10,MB11,MB12,
                            LINK,AC01,AC02,AC03,AC04,AC05,AC06,AC07,AC08,AC09,AC10,AC11,AC12,
                            SC01,SC02,SC03,SC04,SC05,MQ01,MQ02,MQ03,MQ04,MQ05,MQ06,MQ07,MQ08,MQ09,MQ10,MQ11,MQ12,
                            AND,TAD,ISZ,DCA,JMS,JMP,IOT,OPR,
                            FETCH,EXECUTE,DEFER,WORD_COUNT,CURRENT_ADDRESS,BREAK,
                            ION,PAUSE,RUN };

   // array of LED labels; make sure the order here matches the position order in the path array!
   const char* ledLabel[89];
   ledLabel[0] = "DF1";
   ledLabel[1] = "DF2";
   ledLabel[2] = "DF3";
   ledLabel[3] = "IF1";
   ledLabel[4] = "IF2";
   ledLabel[5] = "IF3";
   ledLabel[6] = "PC01";
   ledLabel[7] = "PC02";
   ledLabel[8] = "PC03";
   ledLabel[9] = "PC04";
   ledLabel[10] = "PC05";
   ledLabel[11] = "PC06";
   ledLabel[12] = "PC07";
   ledLabel[13] = "PC08";
   ledLabel[14] = "PC09";
   ledLabel[15] = "PC10";
   ledLabel[16] = "PC11";
   ledLabel[17] = "PC12";
   ledLabel[18] = "MA01";
   ledLabel[19] = "MA02";
   ledLabel[20] = "MA03";
   ledLabel[21] = "MA04";
   ledLabel[22] = "MA05";
   ledLabel[23] = "MA06";
   ledLabel[24] = "MA07";
   ledLabel[25] = "MA08";
   ledLabel[26] = "MA09";
   ledLabel[27] = "MA10";
   ledLabel[28] = "MA11";
   ledLabel[29] = "MA12";
   ledLabel[30] = "MB01";
   ledLabel[31] = "MB02";
   ledLabel[32] = "MB03";
   ledLabel[33] = "MB04";
   ledLabel[34] = "MB05";
   ledLabel[35] = "MB06";
   ledLabel[36] = "MB07";
   ledLabel[37] = "MB08";
   ledLabel[38] = "MB09";
   ledLabel[39] = "MB10";
   ledLabel[40] = "MB11";
   ledLabel[41] = "MB12";
   ledLabel[42] = "LINK";
   ledLabel[43] = "AC01";
   ledLabel[44] = "AC02";
   ledLabel[45] = "AC03";
   ledLabel[46] = "AC04";
   ledLabel[47] = "AC05";
   ledLabel[48] = "AC06";
   ledLabel[49] = "AC07";
   ledLabel[50] = "AC08";
   ledLabel[51] = "AC09";
   ledLabel[52] = "AC10";
   ledLabel[53] = "AC11";
   ledLabel[54] = "AC12";
   ledLabel[55] = "SC01";
   ledLabel[56] = "SC02";
   ledLabel[57] = "SC03";
   ledLabel[58] = "SC04";
   ledLabel[59] = "SC05";
   ledLabel[60] = "MQ01";
   ledLabel[61] = "MQ02";
   ledLabel[62] = "MQ03";
   ledLabel[63] = "MQ04";
   ledLabel[64] = "MQ05";
   ledLabel[65] = "MQ06";
   ledLabel[66] = "MQ07";
   ledLabel[67] = "MQ08";
   ledLabel[68] = "MQ09";
   ledLabel[69] = "MQ10";
   ledLabel[70] = "MQ11";
   ledLabel[71] = "MQ12";
   ledLabel[72] = "AND";
   ledLabel[73] = "TAD";
   ledLabel[74] = "ISZ";
   ledLabel[75] = "DCA";
   ledLabel[76] = "JMS";
   ledLabel[77] = "JMP";
   ledLabel[78] = "IOT";
   ledLabel[79] = "OPR";
   ledLabel[80] = "FETCH";
   ledLabel[81] = "EXECUTE";
   ledLabel[82] = "DEFER";
   ledLabel[83] = "WORD COUNT";
   ledLabel[84] = "CURRENT ADDRESS";
   ledLabel[85] = "BREAK";
   ledLabel[86] = "ION";
   ledLabel[87] = "PAUSE";
   ledLabel[88] = "RUN";

   // clear panel first
   leds_off();

   printf("\nPress ENTER after confirming each LED to continue on to the next one.\n");

   // animate the specified pattern (one LED at a time)
   for ( arrayIdx = 0 ; arrayIdx < sizeof(ledTestArray) ; arrayIdx++ )
   {
      ledstatus[ledRow] = 0;
      ledstatus[ledRow=(ledTestArray[arrayIdx]>>4) - 1] = 04000 >> ((ledTestArray[arrayIdx]&0x0F) - 1);
      printf("LED #[%02i]: %s ", arrayIdx+1, ledLabel[arrayIdx]);
      (void)getchar();
      // put code here to abort the test if the user enters Q?
   }

   leds_off(); // turn off that last pesky LED
   printf("\nTest complete!\n");

}
