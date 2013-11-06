//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#ifndef SYSC_4001_Assignment_2_main_h
#define SYSC_4001_Assignment_2_main_h

#define RAND_SEED (unsigned) time(NULL)

#include <sys/types.h>
pid_t fork_producer(int iteration, int producer_id);
pid_t fork_consumer(int iteration, int consumer_id);

#endif
