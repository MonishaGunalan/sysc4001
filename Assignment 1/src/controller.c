//
//  SYSC 4001 - Assignment 1
//  Monisha Gunalan (100871444)
//  Korey Conway (100838924)
//  October 2013
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include "common.h"
#include "controller.h"

int main(int argc, const char * argv[])
{
	// Init
	if (false == parent_init()) {
		dump("Finshed");
		return 1;
	}
	
	// Setup child/parent process functions
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
	
	// For the 2 processes, and handle the signals and main loops
	start_fork(parent, child);

    return 0;
}

bool parent_init() {
	// Create controller FIFO
	if (-1 == mkfifo(CONTROLLER_FIFO_NAME, 0777)) {
		dump("Failed to create controller FIFO with error: %d", errno);
		return false;
	}
	
	// Create message queue
	msg_queue_id = msgget(MESSAGE_QUEUE_KEY, 0666 | IPC_CREAT);
	if (-1 == msg_queue_id) {
		dump("Failed to setup mesage queue with error: %d", errno);
		unlink(CONTROLLER_FIFO_NAME);
		return false;
	}
	
	return true;
}

void parent_loop() {
	dump("Waiting to receive request through FIFO");
	
	// Open FIFO (blocks until monitor connects)
	controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_RDONLY);
	if (-1 == controller_fifo_fd) {
		dump("[warn] Failed to open fifo with error: %d", errno);
		return;
	}

	// Read from controller FIFO
	fifo_data fdata;
	if (-1 == read(controller_fifo_fd, &fdata, sizeof(fdata))) {
		// Error reading from fifo, just ignore and continue next request
		return;
	}
	
	switch(fdata.request_type) {
		case CONNECT:
			dump("Received request to start from PID: %d", fdata.source_pid);

			// Open FIFO for monitor to respond
			char monitor_fifo_name[MONITOR_FIFO_NAME_MAX_LENGTH];
			sprintf(monitor_fifo_name, MONITOR_FIFO_NAME, fdata.source_pid);
			dump("Opening fifo %s to respond.", monitor_fifo_name);
			int monitor_fifo_fd = open(monitor_fifo_name, O_WRONLY);
			if (-1 == monitor_fifo_fd) {
				dump("Failed to open fifo with error: %d", errno);
				return; // continue looping
			}

			// Add the PID to the array and increment count
			monitor_pid[monitor_count++] = fdata.source_pid;

			// Write response
			dump("Responding with start command");
			fdata.source_pid = get_child_pid();
			fdata.request_type = START;
			if(-1 == write(monitor_fifo_fd, &fdata, sizeof(fdata))) {
				dump("Failed to write to fifo");
				close(monitor_fifo_fd);
				return; // continue looping
			}
			
			close(monitor_fifo_fd);
			
			return;
		default:
			dump("Received unknown request. Ignoring it.");
			return;
	}
}

void parent_cleanup() {
	for(int i = 0; i < monitor_count; i++) {
		close_monitor(monitor_pid[i]);
	}
	
	unlink(CONTROLLER_FIFO_NAME);
	dump("Finished");
}

void close_monitor(int monitor_pid) {
	// Open FIFO for monitor to respond
	char monitor_fifo_name[MONITOR_FIFO_NAME_MAX_LENGTH];
	sprintf(monitor_fifo_name, MONITOR_FIFO_NAME, monitor_pid);
	dump("Opening fifo %s to send terminate request.", monitor_fifo_name);
	int monitor_fifo_fd = open(monitor_fifo_name, O_WRONLY);
	if (-1 == monitor_fifo_fd) {
		dump("Failed to open fifo with error: %d", errno);
		return;
	}
	
	// Write response
	dump("Sending request to terminate to monitor pid: %d", monitor_pid);
	fifo_data fdata;
	fdata.source_pid = getpid();
	fdata.request_type = STOP;
	if(-1 == write(monitor_fifo_fd, &fdata, sizeof(fdata))) {
		dump("Failed to write to fifo");
	}
	
	close(monitor_fifo_fd);
}


bool child_setup() {
	msg_queue_id = msgget(MESSAGE_QUEUE_KEY, 0666 | IPC_CREAT);
	if (msg_queue_id == -1) {
		// Invalid message queue
		dump("Failed to open/create controller queue.");
		send_sigterm_to_parent(); // Tell parent to stop
		return false;
	}
	return true;
}

void child_loop() {
	msg_data msg;
	
	// Read from message queue
	dump("Reading from the message queue");
	msgrcv(msg_queue_id, &msg, sizeof(msg.data), getpid(), 0);

	dump("Received heartbeat: %d from %s", msg.data.heartbeat, msg.data.patient_name);

	dump("Sending ack back to monitor");
	msg_data response_msg = {
		.destination_key = msg.data.source_key,
		.data = {
			.source_key = getpid(),
			.heartbeat = 0,
			.ack = true,
			.patient_name = "",
		},
	};
	msgsnd(msg_queue_id, &response_msg, sizeof(msg.data), 0);
}

void child_cleanup() {
	dump("Finished");
}

