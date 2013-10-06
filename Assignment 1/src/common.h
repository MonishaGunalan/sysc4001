//
//  SYSC 4001 - Assignment 1
//  Monisha Gunalan (100871444)
//  Korey Conway (100838924)
//  October 2013
//

#ifndef Assignment_1_common_h
#define Assignment_1_common_h

// Public Functions
void setup_common(bool* running, void(*parent_main)(void), void(*child_main)(void));

// Private Functions
static void setup_signal_handling(void);
static void handle_signal(int sigid);
static bool is_parent();
static void terminate();

// Private variables
static pid_t cm_child_pid;
static bool* cm_running;

#endif
