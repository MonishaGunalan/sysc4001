//
//  main.c
//  monitor
//
//  Created by Korey Conway on 2013-10-01.
//  Copyright (c) 2013 SG•Nexus. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdbool.h>

#define HEARTBEAT_INTERVAL 2
#define HEARTBEAT_MAX 100
#define HEARTBEAT_MIN 60
#define RAND_SEED (unsigned) time(NULL)

void parent_main(void);
void child_main(void);
void handle_signal(int sigid);
bool is_parent(void);

pid_t child_pid;
int terminating = 0; // set to 1 to tell child to stop

struct fifo_data {
	int heartbeat_rate;
	int pid;
} controller_data;

int main(int argc, const char * argv[])
{
	// Setup signal listening
	struct sigaction act = {
		.sa_handler = handle_signal,
		.sa_flags = 0
	};
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
	
	// Fork the child
	child_pid = fork();
	switch(child_pid) {
		case -1: // fork failed
			perror("Fork failed");
			exit(1);
		case 0: // child
			child_main();
			break;
		default: // parent
			parent_main();
			break;
	}
	
	return 0;
}

void parent_main() {
	while(!terminating) {
		printf("I am parent\n");
		sleep(1);
	}
	
	printf("\nParent terminated\n");
}

void child_main() {
	// Intializes random number generator
	srand(RAND_SEED);
	
	// Child's main loop
	while(!terminating) {
		// Generate random number for heartbeat rate
		int r = (rand() %(HEARTBEAT_MAX - HEARTBEAT_MIN)) + HEARTBEAT_MIN;
		printf("Heartbeat: %d \n", r);
		sleep(HEARTBEAT_INTERVAL);
	}

	printf("\nChild terminated\n");
}

bool is_parent() {
	return (child_pid > 0);
}

void terminate() {
	terminating = 1;

	if (is_parent()) {
		// Send signal to child to close
		kill(child_pid, SIGTERM);
	}
}

void handle_signal(int sigid) {
	switch(sigid) {
		case SIGTERM:
		case SIGINT:
			terminate();
		default:
			// Ignore other signals
			break;
	}
}


