#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef unsigned int    uint32; 
typedef unsigned char   uint8;

extern void *blink(void *ptr);	// the real-time multiplexing process to start up
extern uint32 ledstatus[8];     // bitfields: 8 ledrows of up to 12 LEDs
extern uint32 switchstatus[3];  // bitfields: 3 rows of up to 12 switches

uint32 lastswitchstatus[3];     // to watch for switch changes

#include <signal.h>
#include <ctype.h>

uint8 path[] = { 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x11, 0x12, 0x13, 0x14, 0x15,
		 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x2c, 0x2b, 0x2a, 0x29,
		 0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x31, 0x32, 0x33,
		 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x4c, 0x4b,
		 0x4a, 0x49, 0x48, 0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 0x87,
		 0x76, 0x77, 0x78, 0x79, 0x7a, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
		 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x68, 0x67, 0x66, 0x65, 0x64,
		 0x63, 0x62, 0x61, 0x69, 0x6a, 0x6b, 0x6c, 0x71, 0x72, 0x75, 0x74,
		 0x73 };

#define CHECK(cond,name) while(!(cond)) goto CLN_ ## name;
#define CLEANUP(name)    if( 0 ) { CLN_ ## name: {
#define CLEANUP_END      } } else;

int terminate=0;

void sig_handler( int signo )
{
  if( signo == SIGINT )
    terminate = 1;
}


int main( int argc, char *argv[] )
{
  pthread_t thread1;
  int iret1;

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

  sleep( 1 );			// allow multiplex to start

  CHECK( !terminate, TERMINATE )

  CLEANUP( TERMINATE )

    if( pthread_join(thread1, NULL) )
      printf( "\r\nError joining multiplex thread\r\n" );
    return 0;

  CLEANUP_END
}
