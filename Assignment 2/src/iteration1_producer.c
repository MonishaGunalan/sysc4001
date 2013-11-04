//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "producer_common.h"
#include "common.h"

#define PRODUCE_MAX 100
#define

int buffer[5];

int main(int argc, const char * argv[])
{
    start_producers(5, 40);
    return 0;
}

void start_producers(int number_of_producers, int number_of_entries)
{
    // Seed random number generation
    srand(time());
    
    // Create semaphores
    
    // Create producers
    for (int i = 0; i < number_of_producers; i++) {
        fork_producer(i+1, number_of_producers);
    }
}

void fork_producer(int producer_id, int number_of_entries)
{
    int child_pid = fork();
    switch(child_pid) {
        case -1:
            printf("Failed to fork producer");
            break;
        case 0:
            return;
        default:
            printf("Failed to fork producer");
    
    }
    //
}

void start_prod(int producer_id, int number_of_entries) {
    // Wait for buffer to have space
    
    // Add to buffer
    buffer[in_index % BUFFER_SIZE] = generate_producer_value(producer_id);
    
    // Release semaphore
}

int generate_producer_value(int producer_id) {
    return producer_id * 1000 + (rand() % PRODUCE_MAX);
}
