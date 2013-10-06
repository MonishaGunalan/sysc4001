//
// SYSC 4001 - Assignment 1
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// October 2013
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "common.h"

void setup_common(bool* running, void(*parent_main)(), void(*child_main)()) {
	setup_signal_handling();
	
	// Fork the child
	cm_running = running;
	*cm_running = true;
	cm_child_pid = fork();
	switch(cm_child_pid) {
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

}

static bool is_parent() {
	return (cm_child_pid > 0);
}

static void setup_signal_handling() {
	// Setup signal listening
	struct sigaction act = {
		.sa_handler = handle_signal,
		.sa_flags = 0
	};
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
}

static void handle_signal(int sigid) {
	switch(sigid) {
		case SIGTERM:
		case SIGINT:
			terminate();
		default:
			// Ignore other signals
			break;
	}
}

static void terminate() {
	*cm_running = false;
	
	if (is_parent()) {
		// Send signal to child to close
		kill(cm_child_pid, SIGTERM);
	}
}
