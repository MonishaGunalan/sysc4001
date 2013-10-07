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
#include <sys/msg.h>
#include <signal.h>

#include "common.h"
#include "monitor.h"

int main(int argc, const char * argv[])
{
	// Get the patient name
	if (argc <= 1) {
		// Name not given in argv, so prompt user for it
		printf("Patient name: ");
		size_t buff_size;
		char* buffer;
		getline(&buffer, &buff_size, stdin);
		strncpy(patient_name, buffer, NAME_MAX_LENGTH);
	} else {
		// Get name from argv
		strncpy(patient_name, argv[1], NAME_MAX_LENGTH);
	}
	
	dump("Starting monitor for patient: %s", patient_name);
	
	// Setup child/parent processes
	process_t parent = {
		.setup = 0,
		.loop = parent_loop,
		.cleanup = parent_cleanup,
	};
	
	process_t child = {
		.setup = child_setup,
		.loop = child_loop,
		.cleanup = child_cleanup,
	};
	
	start_processes(parent, child);
	
	return 0;
}

void parent_loop() {
	// Parent's main loop
	dump("I am parent");
	sleep(1);
}

void parent_cleanup() {
	dump("Parent finished");
}

bool child_setup() {
	// Child setup
	srand(RAND_SEED); // Intializes random number generator

	// Get message queues
	controller_queue_id = msgget(CONTROLLER_MESSAGE_QUEUE_KEY, 0666);
	monitor_queue_id = msgget(MONITOR_MESSAGE_QUEUE_KEY, 0666);
		
	// Verify queues are valid
	if (controller_queue_id == -1) {
		dump("Cannot open controller queue. Is controller running?");
		send_sigterm_to_parent(); // Kill the monitor
		return false;
	} else if (monitor_queue_id == -1) {
		dump("Cannot open monitor queue. Is controller running?");
		send_sigterm_to_parent(); // Kill the monitor
		return false;
	}

	// Initialize msg to be sent to controller
	msg_to_send.msg_key = getpid();
	return true;
}

void child_loop() {
	// Generate random number for heartbeat rate
	int heartbeat = (rand() %(HEARTBEAT_MAX - HEARTBEAT_MIN)) + HEARTBEAT_MIN;
	dump("Heartbeat: %d ", heartbeat);
	
	// Send to controller's message queue
	dump("Sending heartbeat to controller");
	msg_to_send.heartbeat = heartbeat;
	msgsnd(controller_queue_id, &msg_to_send, sizeof(msg_to_send.heartbeat), 0);
	
	// Wait for ack
	dump("Waiting for ACK");
	msgrcv(monitor_queue_id, &received_msg, sizeof(received_msg.ack), getpid(), 0);
	if (received_msg.ack) {
		dump("ACK received");
	} else {
		dump("ACK received with error");
	}
	
	// Sleep for hearbeat interval
	sleep(HEARTBEAT_INTERVAL);
}

void child_cleanup() {
	dump("Child finished");
}

