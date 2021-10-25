/* *********************************************************************
* Receive signals over USB port and display them
*
*  *********************************************************************
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

/* Timer spec*/
timer_t timer = (timer_t) 0;
long int periode = 100 ;
struct sigevent event;
struct itimerspec spec;

/* Serial port */
#define BDR B9600
int USB = 0;
struct termios tty;
struct termios tty_old;
int n = 0 ;
char buf[80];

/* serial port configuration */
void configure_serialport(void) ;

/* handler of serial port */
void handler_serialport(int no) ;

/* ********************************************************************
* Main thread
* 
* *********************************************************************
*/
int main (void)
{
	configure_serialport() ;
	
	/* Configurer le timer */
	signal (SIGRTMIN, handler_serialport);
	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo  = SIGRTMIN;
	periode = periode*1000000 ;	/* conversion en ns */
	spec.it_interval.tv_sec  = periode/1000000000;
	spec.it_interval.tv_nsec = periode % 1000000000;	
	spec.it_value = spec.it_interval;
	/* Allouer le timer */
	if (timer_create(CLOCK_REALTIME, & event, & timer) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	} 
	/* Programmer le timer */
	if (timer_settime(timer, 0, &spec, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
	
	/*  Pause until occurrence of timer*/
	while (1) {
		pause(); 
	}
	close(USB) ;
	return 0;
}

/* ********************************************************************
* Configure serial port
* 
* *********************************************************************
*/
void configure_serialport(void)
{
	/* Open port */
	// int USB = open( "/dev/ttyUSB0", O_RDWR);
	USB = open( "/dev/ttyACM0", O_RDWR | O_NONBLOCK );
	if (USB < 0) {
    printf("Error %i from open: %s\n", errno, strerror(errno));
	}
	
	/* Set parameters */
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 ) {
	   printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)BDR);
	cfsetispeed (&tty, (speed_t)BDR);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;            // Make 8n1: disable parity bit
	tty.c_cflag     &=  ~CSTOPB;			// one stop bit is used
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;				// 8 bits per byte

	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
	
	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
	   printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
}

void handler_serialport(int no)
{
/*	printf("%d\n",++compteur);
	unsigned char msg[] = "s\r\n" ;
	write (USB, msg, sizeof(msg));
	tcflush( USB, TCIFLUSH ); */
	n = read( USB, &buf, sizeof buf ); 
	printf("%s", buf);
}
