//
// SYSC 4001 - Assignment 1
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// October 2013
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "common.h"
#include "monitor.h"

int main(int argc, const char * argv[])
{
	setup_common(&running, parent_main, child_main);
	return 0;
}

void parent_main() {
	// Parent setup
	
	// Parent's main loop
	while(running) {
		printf("I am parent\n");
		sleep(1);
	}
	
	printf("\nParent terminated\n");
}

void child_main() {
	// Child setup
	srand(RAND_SEED); // Intializes random number generator
	
	// Child's main loop
	while(running) {
		// Generate random number for heartbeat rate
		int r = (rand() %(HEARTBEAT_MAX - HEARTBEAT_MIN)) + HEARTBEAT_MIN;
		printf("Heartbeat: %d \n", r);
		sleep(HEARTBEAT_INTERVAL);
	}

	printf("\nChild terminated\n");
}




