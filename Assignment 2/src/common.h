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
#define ENTRIES_PER_PRODUCER 20

pid_t fork_child(int iteration, int id, void (*callback)(int));
void start_children(int iteration, int child_count, void (*callback)(int));
int generate_producer_value(int producer_id);

#endif
