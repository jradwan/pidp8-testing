/*
 *****************************************************************************
 *
 * Purpose:
 * DEEPER THOUGHT
 *  - This program simulates the appearance of the famous program
 *    deep thought for the PDP-8, on the PiDP-8 kit created by
 *    Oscar Vermeulen -- see his kit at
 *       http://obsolescence.wix.com/obsolescence#!pidp-8
 *    The operation of this program was copied from the programs I've seen
 *    on videos from YouTube
 *
 * This program was created because the "full blown" simH PDP-8 emulator
 * uses 100% of the CPU.  This application only uses 12%
 *
 * NOTE: This program must be run as superuser
 *
 * The file gpio.c and gpio.h were copied from Oscar's PDP-8/simH project
 *
 * Modification Log:
 * Rev.  Date       By              Description
 * ------------------------------------------------------------------------
 * 1.0   2016.02.23 Norman Davie    Initial release
 *****************************************************************************
 */

#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef unsigned int    uint32;
typedef signed int      int32;
typedef unsigned short  uint16;
typedef unsigned char   uint8;

extern void *blink(void *ptr);	// the real-time multiplexing process to start up
extern uint32 ledstatus[8];     // bitfields: 8 ledrows of up to 12 LEDs
extern uint32 switchstatus[3];  // bitfields: 3 rows of up to 12 switches


#include <signal.h>
#include <ctype.h>

// GET / STORE             row   shift  mask value
int programCounter[] 	= {0x00, 0,     07777};
int dataField[] 	= {0x07, 9,     0777};
int instField[] 	= {0x07, 6,     0777};
int linkLED[]           = {0x07, 5,     01};
int memoryAddress[]     = {0x01, 0,     07777};
int memoryBuffer[]      = {0x02, 0,     07777};
int accumulator[]       = {0x03, 0,     07777};
int multiplierQuotient[]= {0x04, 0,     07777};
int andLED[]            = {0x05, 11,    01};
int tadLED[]            = {0x05, 10,    01};
int iszLED[]            = {0x05, 9,     01};
int dcaLED[]            = {0x05, 8,     01};
int jmsLED[]            = {0x05, 7,     01};
int iotLED[]            = {0x05, 5,     01};
int jmpLED[]            = {0x05, 6,     01};
int oprLED[]            = {0x05, 4,     01};
int fetchLED[]          = {0x05, 3,     01};
int executeLED[]        = {0x05, 2,     01};
int deferLED[]          = {0x05, 1,     01};
int wordCountLED[]      = {0x05, 0,     01};
int currentAddressLED[] = {0x06, 11,    01};
int breakLED[]          = {0x06, 10,    01};
int ionLED[]            = {0x06, 9,     01};
int pauseLED[]          = {0x06, 8,     01};
int runLED[]            = {0x06, 7,     01};
int stepCounter[]       = {0x06, 0,     0177};

// GETSWOTCJ
int singInst[]          = {0x02, 4,     01};
int singStep[]          = {0x02, 5,     01};
int stop[]              = {0x02, 6,     01};
int cont[]              = {0x02, 7,     01};
int exam[]              = {0x02, 8,     01};
int dep[]               = {0x02, 9,     01};
int loadAdd[]           = {0x02, 10,    01};
int start[]             = {0x02, 11,    01};

// GETSWITCHES
int swregister[]        = {0x00, 0,     07777};
int step[]              = {0x01, 6,     077};

// STORE 
// 1) clamps the maximum value via an and mask
// 2) shifts the value to the appropriate area of within the uint
// 3) masks out the value that was previously there without effecting other bits
// 4) or's the new value in place
#define STORE(item, value) { ledstatus[item[0]] =  (ledstatus[item[0]] & ~(item[2] << item[1])  ) |  ((value & item[2]) << item[1]); }

// GET
// 1) gets shifts the value to the "normal" range
// 2) masks off bits that are not related to our value
#define GET(item)          ( (ledstatus    [ item[0] ] >> item[1]) & item[2] )
#define GETSWITCH(flip)   !( (switchstatus [ flip[0] ] >> flip[1]) & flip[2] )
#define GETSWITCHES(flip)  ( (switchstatus [ flip[0] ] >> flip[1]) & flip[2] )


int terminate=0;


// Handle CTRL-C
void sig_handler( int signo )
{
  if( signo == SIGINT )
    terminate = 1;
}




int main( int argc, char *argv[] )
{
  pthread_t     thread1;
  int           iret1;
  unsigned long sleepTime;
  int           deeperThoughMode = 0;
  int           dontChangeLEDs = 0;
  unsigned long delayAmount;
  unsigned long varietyAmount;

  // install handler to terminate future thread
  if( signal(SIGINT, sig_handler) == SIG_ERR )
    {
      fprintf( stderr, "Failed to install SIGINT handler.\n" );
      exit( EXIT_FAILURE );
    }

  // create thread
  iret1 = pthread_create( &thread1, NULL, blink, &terminate );

  if( iret1 )
    {
      fprintf( stderr, "Error creating thread, return code %d\n", iret1 );
      exit( EXIT_FAILURE );
    }

  sleep( 2 );			// allow 2 sec for multiplex to start

  srand(time(NULL));

  // set the status LEDs
  STORE(ionLED,     1);
  STORE(fetchLED,   1);
  STORE(executeLED, 1);
  STORE(runLED,     1);
  STORE(pauseLED,   0);
  STORE(jmpLED,     1);

  while(! terminate)
  {
    // blink the execute LED after every randomization
    STORE(executeLED, ! GET(executeLED));

    // if we're paused -- don't change the LEDs
    if (! dontChangeLEDs)
    {

      // In future revisions, we'll have different randomization sequences
      switch(deeperThoughMode)
      {
      case -1:
        break;
      default:
        STORE(programCounter,    rand() & programCounter[2]);
        STORE(memoryAddress,     rand() & memoryAddress[2]);
        STORE(memoryBuffer,      rand() & memoryBuffer[2]);
        STORE(accumulator,       rand() & accumulator[2]);
        STORE(multiplierQuotient,rand() & multiplierQuotient[2]);
        STORE(stepCounter,       rand() & stepCounter[2]);
        STORE(dataField,         rand() & dataField[2]);
        STORE(instField,         rand() & instField[2]);
        break;
      }

      // Maximum amount to delay between changes
      // least signifiant address lines control the maximum delay
      // all "up" -- maximum delay
      // all "down" -- no delay
      delayAmount  =  (GETSWITCHES(swregister) & 07) * 400000L;

      // How much to vary the above timing
      // the next bank of three address lines control how much
      // we can shorten the maximum delay 
      // all "up" -- we can shorten to zero seconds
      // all "down" -- must use maximum time before we change 
      varietyAmount = (unsigned long) (((rand() & delayAmount) / 7.0f) * ( (GETSWITCHES(swregister) & 070)>>3));

      sleepTime = delayAmount - varietyAmount;
    }
    else
     sleepTime = 250 * 1000;


    usleep(sleepTime);

    // if the stop switch is selected, then clean up nicely
    if (GETSWITCH(stop)) terminate = 1;

    // if one of the single step switches is selected, then "pause" and don't change the LED display
    // otherwise "run"
    dontChangeLEDs = GETSWITCH(singStep) || GETSWITCH(singInst);
    STORE(pauseLED, dontChangeLEDs);
    STORE(runLED, ! dontChangeLEDs);
 }


  if( pthread_join(thread1, NULL) )
    printf( "\r\nError joining multiplex thread\r\n" );

  return 0;
}
