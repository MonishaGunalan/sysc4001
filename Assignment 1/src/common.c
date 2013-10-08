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

void start_fork(process_t parent, process_t child) {
	// Fork the child
	child_pid = fork();
	switch(child_pid) {
		case -1: // fork failed
			perror("Fork failed");
			exit(1);
		case 0: // child
			setup_signal_handling();
			run_process(child);
			break;
		default: // parent
			setup_signal_handling();
			run_process(parent);
			break;
	}
}

void send_sigterm_to_parent() {
	dump("Sending SIGTERM to parent process");

	if (is_parent()) {
		kill(getpid(), SIGTERM);
	} else {
		kill(getppid(), SIGTERM);
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

int get_child_pid() {
	return (int)child_pid;
}

static void run_process(process_t process) {
	bool setup_ok = (process.setup) ? process.setup() : true;
	
	if (setup_ok && process.loop) {
		running = true;
		while(running) {
			process.loop();
		}
	}
	
	if (process.cleanup) {
		process.cleanup();
	}
}

static bool is_parent() {
	return (child_pid != 0);
}

static void setup_signal_handling() {
	// Setup signal listening
	dump("Setup signal handling");

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
		sigaction(SIGINT, &ignore_act, 0);
		sigaction(SIGTERM, &handle_act, 0);
	}
}

static void handle_signal(int sigid) {
	if (SIGTERM == sigid) {
		dump("Received SIGTERM");
		end_loop();
	} else if (SIGINT == sigid) {
		dump("Received SIGINT");
		end_loop();
	} else {
		// Ignore other signals
		dump("Received unknown signal: %d", sigid);
	}
}

static void end_loop() {
	running = false;
	
	if (is_parent() && child_pid != -1) {
		// Send signal to child to close
		dump("Sending SIGTERM to child process");
		kill(child_pid, SIGTERM);
	}
}
