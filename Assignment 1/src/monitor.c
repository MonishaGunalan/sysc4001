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
#include <string.h>

#include "common.h"
#include "monitor.h"

int main(int argc, const char * argv[])
{
	// Get the patient name
	if (argc <= 1) {
		printf("Patient name: ");
		size_t buff_size;
		char* buffer;
		getline(&buffer, &buff_size, stdin);
		strncpy(patient_name, buffer, NAME_MAX_LENGTH);
	} else {
		strncpy(patient_name, argv[1], NAME_MAX_LENGTH);
	}
	
	dump("Starting monitor for patient: %s", patient_name);
	
	// Setup child/parent processes
	setup_common(&running, parent_main, child_main);
	return 0;
}

void parent_main() {
	// Parent setup
	
	// Parent's main loop
	while(running) {
		dump("I am parent");
		sleep(1);
	}
	
	dump("Parent terminated");
}

void child_main() {
	// Child setup
	srand(RAND_SEED); // Intializes random number generator
	
	// Child's main loop
	while(running) {
		// Generate random number for heartbeat rate
		int r = (rand() %(HEARTBEAT_MAX - HEARTBEAT_MIN)) + HEARTBEAT_MIN;
		dump("Heartbeat: %d ", r);
		sleep(HEARTBEAT_INTERVAL);
	}

	dump("Child terminated");
}




