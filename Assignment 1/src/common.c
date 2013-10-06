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
#include <stdarg.h>
#include "common.h"

void setup_common(bool* running, void(*parent_main)(), void(*child_main)()) {
	// Fork the child
	cm_running = running;
	*cm_running = true;
	cm_child_pid = fork();
	switch(cm_child_pid) {
		case -1: // fork failed
			perror("Fork failed");
			exit(1);
		case 0: // child
			setup_signal_handling();
			child_main();
			break;
		default: // parent
			setup_signal_handling();
			parent_main();
			break;
	}
}

void dump(const char * format, ...) {
	va_list args;
	va_start(args, format);
	
	if (is_parent()) {
		printf("[parent] ");
	} else {
		printf("[child]  ");
	}
	
	vprintf(format, args);
	printf("\n");
	
	va_end(args);
}

static bool is_parent() {
	return (cm_child_pid != 0);
}

static void setup_signal_handling() {
	// Setup signal listening
	struct sigaction handle_act = {
		.sa_handler = handle_signal,
		.sa_flags = 0
	};
	sigemptyset(&handle_act.sa_mask);
	
	if (is_parent()) {
		sigaction(SIGINT, &handle_act, 0);
		sigaction(SIGTERM, &handle_act, 0);
	} else {
		struct sigaction ignore_act = {
			.sa_handler = SIG_IGN,
			.sa_flags = 0
		};
		sigemptyset(&ignore_act.sa_mask);
//		sigaction(SIGINT, &ignore_act, 0);
		sigaction(SIGINT, &handle_act, 0);
		sigaction(SIGTERM, &handle_act, 0);
	}
}

static void handle_signal(int sigid) {
	switch(sigid) {
		case SIGTERM:
		case SIGINT:
			if (sigid == SIGTERM) {
				dump("Received SIGTERM");
			} else if (sigid == SIGINT) {
				dump("Received SIGINT");
			}
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
		dump("Sending SIGTERM to child process");
		//kill(cm_child_pid, SIGTERM);
	}
}
