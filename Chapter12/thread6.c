#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread_function(void *arg);

char message[] = "Hello World~~!";
int thread_finished = 0;

int max_priority;
int min_priority;
struct sched_param scheduling_value;

int main()
{
	int res;
	pthread_t a_thread;

	pthread_attr_t thread_attr;

	res = pthread_attr_init(&thread_attr);
	if (res != 0) {
		error("Attribute creation failed");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_attr_setdetachstate(&thread_attr, SCHED_OTHER);
	if ( res != 0) {
		perror("Setting scheduling policy failed");
		exit(EXIT_FAILURE);
	}

	max_priority = sched_get_priority_max(SCHED_OTHER);
	min_priority = sched_get_priority_min(SCHED_OTHER);

	scheduling_value.sched_priority = min_priority;
	
	res = pthread_attr_setschedparam(&thread_attr, &scheduling_value);
	if (res != 0) {
		perror("Setting scheduling priority failed");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_create(&a_thread, &thread_attr, thread_function, (void *)message);
	if (res != 0) {
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}
	
	(void)pthread_attr_destroy(&thread_attr);
	while ( !thread_finished ) {
		printf("Waiting for thread to say it's finished ... \n");
		sleep(1);
	}

	printf("Othrer thread finished, bye!\n");
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) 
{
	printf("thread_function is running, Argument was %\n", (char *)arg);
	sleep(4);
	printf("Swcond thread setting finished flag, and exit now\n");
	thread_finished = 1;
	pthread_exit(NULL);
}
