
//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#ifndef SYSC_4001_Assignment_2_Alternative2_h
#define SYSC_4001_Assignment_2_Alternative2_h

#include <semaphore.h> // needed for sem_t type

#define SEMAPHORE_N "/sem_n"
#define SEMAPHORE_E "/sem_e"

void run_alternative2(int buffer_size);
void start_producer1(int producer_id);
void start_consumer1(int consumer_id);

#endif
