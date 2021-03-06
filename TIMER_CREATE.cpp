

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

void handler_signal_1(int inutilise)
{
	fprintf(stderr, "1 ");
}

void handler_signal_2(int inutilise)
{
	fprintf(stderr, "2 ");
}


int main(int argc, char * argv[])
{
	timer_t timer_1, timer_2;
	struct sigevent event_1, event_2;
	struct itimerspec spec_1, spec_2;

	// Installer les handlers
	signal(SIGRTMIN+1, handler_signal_1);
	signal(SIGRTMIN+2, handler_signal_2);

	// Indiquer la notification desiree 
	event_1.sigev_notify = SIGEV_SIGNAL;
	event_1.sigev_signo  = SIGRTMIN+1;

	// Configurer la periode du timer
	spec_1.it_interval.tv_sec  = 1;
	spec_1.it_interval.tv_nsec = 0;
	spec_1.it_value = spec_1.it_interval;
	
	// Allouer le timer
	if (timer_create(CLOCK_REALTIME, & event_1, & timer_1) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	} 

	// Memes operations pour le second timer
	event_2.sigev_notify = SIGEV_SIGNAL;
	event_2.sigev_signo  = SIGRTMIN+2;
	spec_2.it_interval.tv_sec  = 0;
	spec_2.it_interval.tv_nsec = 250000000; // 0,25 sec.
	spec_2.it_value = spec_2.it_interval;
	if (timer_create(CLOCK_REALTIME, & event_2, & timer_2) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	} 

	// Programmer les timers
	if ((timer_settime(timer_1, 0, &spec_1, NULL) != 0)
	 || (timer_settime(timer_2, 0, &spec_2, NULL) != 0)) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
	while (1) 
		pause();
	return EXIT_SUCCESS;
}

