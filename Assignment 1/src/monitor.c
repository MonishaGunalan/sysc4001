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
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
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
	
	if (false == parent_init()) {
		dump("Finished");
		return 1;
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
	
	start_fork(parent, child);
	
	return 0;
}

bool parent_init() {
	// Open FIFO to controller
	dump("Opening controller FIFO");
	int controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_WRONLY);
	if (-1 == controller_fifo_fd) {
		dump("Failed to open controller fifo with error: %d", errno);
		return false;
	}
	
	// Create FIFO for listening for this monitor
	sprintf(monitor_fifo_name, MONITOR_FIFO_NAME, getpid());
	dump("Creating fifo for monitor to read: %s", monitor_fifo_name);
	if (-1 == mkfifo(monitor_fifo_name, 0777)) {
		unlink(monitor_fifo_name);
		dump("Failed to create fifo with error: %d", errno);
		return false;
	}

	// Send request to connect with controller
	dump("Sending request to connect with controller");
	fifo_data fdata;
	fdata.source_pid = getpid();
	fdata.request_type = CONNECT;
	write(controller_fifo_fd, &fdata, sizeof(fdata));
	
	// Wait to get response from controller
	dump("Waiting for response from controller");
	int monitor_fifo_fd = open(monitor_fifo_name, O_RDONLY);
	if (-1 == monitor_fifo_fd) {
		unlink(monitor_fifo_name);
		dump("Failed to open monitor fifo with error: %d", errno);
		return false;
	}
	
	// Read the response
	if (-1 == read(monitor_fifo_fd, &fdata, sizeof(fdata))) {
		unlink(monitor_fifo_name);
		dump("Failed to read monitor fifo with error: %d", errno);
		return false;
	}
	
	// Check if we got the go ahead
	switch(fdata.request_type) {
		case START:
			controller_key = fdata.source_pid;
			dump("Received start command from controller");
			return true;
		default:
			dump("Invalid response received from controller");
			return false;
	}
	
	return true;
}

void parent_loop() {
	// Wait to get instruction from controller
	dump("Waiting for instruction from controller");
	int monitor_fifo_fd = open(monitor_fifo_name, O_RDONLY);
	if (-1 == monitor_fifo_fd) {
		unlink(monitor_fifo_name);
		dump("Failed to open monitor fifo with error: %d", errno);
		return;
	}
	
	// Read the response
	fifo_data fdata;
	if (-1 == read(monitor_fifo_fd, &fdata, sizeof(fdata))) {
		unlink(monitor_fifo_name);
		dump("Failed to read monitor fifo with error: %d", errno);
		return;
	}
	
	// Check if we got the go ahead
	switch(fdata.request_type) {
		case STOP:
			dump("Received stop command from controller");
			send_sigterm_to_parent();
			return;
		default:
			dump("Invalid response received from controller");
			return;
	}
}

void parent_cleanup() {
	// Delete the monitor fifo
	unlink(monitor_fifo_name);
	dump("Parent finished");
}

bool child_setup() {
	// Child setup
	srand(RAND_SEED); // Intializes random number generator

	// Get message queue
	msg_queue_id = msgget(MESSAGE_QUEUE_KEY, 0666);
	if (msg_queue_id == -1) {
		// Invalid message queue
		dump("Cannot open controller queue. Is controller running?");
		send_sigterm_to_parent(); // Tell parent to stop
		return false;
	}

	return true;
}

void child_loop() {
	// Generate random number for heartbeat rate
	int heartbeat = (rand() %(HEARTBEAT_MAX - HEARTBEAT_MIN)) + HEARTBEAT_MIN;
	dump("Heartbeat: %d ", heartbeat);
	
	// Send to controller's message queue
	dump("Sending heartbeat to controller");
	msg_data msg = {
		.destination_key = controller_key,
		.data = {
			.source_key = getpid(),
			.heartbeat = heartbeat,
			.ack = false,
		}
	};
	strcpy(msg.data.patient_name, patient_name);
	
	if (-1 == msgsnd(msg_queue_id, &msg, sizeof(msg.data), 0)) {
		dump("Error sending message in queue");
		return;
	}
	
	// Wait for ack
	dump("Waiting for ACK");
	if (-1 == msgrcv(msg_queue_id, &msg, sizeof(msg.data), getpid(), 0)) {
		dump("Error receiving message in queue");
		return;
	}

	if (msg.data.ack) {
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

