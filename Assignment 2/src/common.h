//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#ifndef SYSC_4001_Assignment_2_common_h
#define SYSC_4001_Assignment_2_common_h

#include <sys/types.h> // needed for pid_t type
#include <time.h>

#define RAND_SEED (unsigned) time(NULL)
#define ENTRIES_PER_PRODUCER 500
#define SLEEP_UTIME 800 // micro-seconds to sleep in critical sections

pid_t fork_child(int alternative, int id, void (*callback)(int));
void start_children(int alternative, int child_count, void (*callback)(int));
int generate_producer_value(int producer_id);

#endif
