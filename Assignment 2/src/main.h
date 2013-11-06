//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#ifndef SYSC_4001_Assignment_2_main_h
#define SYSC_4001_Assignment_2_main_h

#define SEMAPHORE_S "/semaphore_s"
#define SEMAPHORE_N "/semaphore_n"
#define SEMAPHORE_E "/semaphore_e"

#define RAND_SEED (unsigned) time(NULL)


int in_index = 0;
int out_index = 0;

void buffer_init(int size);
int  buffer_get_size(void);
int  buffer_get_value(int index);
void buffer_set_value(int index, int value);

void run_alternative1(int buffer_size, int number_of_producers, int number_of_consumers);
void start_producer1(int producer_id);
pid_t fork_producer(int iteration, int producer_id);

#endif
