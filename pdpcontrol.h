// pdpcontrol.h
// PiDP-8 control functions

#include <pthread.h>
#include <signal.h>

int terminate = 0;
pthread_t thread1;

extern void *blink(void *ptr);  // the real-time multiplexing process to start up


void sig_handler( int signo )
{
  if( signo == SIGINT )
    terminate = 1;
}

int start_multiplexer ( )
{
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

  sleep( 2 ); // allow multiplex to start
}
