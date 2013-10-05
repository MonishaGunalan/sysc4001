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

#define INTERVAL 2
#define MAX_HEARTBEAT 100
#define MIN_HEARTBEAT 60
#define RAND_SEED (unsigned) time(NULL)

int child_pid;
int child_done = 0; // set to 1 to tell child to stop

struct fifo_data {
	int heartbeat_rate;
	int pid;
} controller_data;

int main(int argc, const char * argv[])
{
	// insert code here...
	printf("Hello, World!\n");
	srand(RAND_SEED);
	for (int i = 0; i < 5; i++) {
		int r = (rand() %(MAX_HEARTBEAT - MIN_HEARTBEAT)) + MIN_HEARTBEAT;
		printf("random number =  %d \n", r);
	}
	return 0;
}

void start_child(struct fifo_data *data) {
	// fork here
    
    // Generate random number for heartbeat rate
	srand(45);
    int r = (rand() %(MAX_HEARTBEAT - MIN_HEARTBEAT)) + MIN_HEARTBEAT;
    printf("random number =  %d \n", r);
}

void end_child() {
	// if we are child
	// close ourselves
	// if we are parent
	// send signal to child to close
}

void create_controller_fifo() {
	
}

