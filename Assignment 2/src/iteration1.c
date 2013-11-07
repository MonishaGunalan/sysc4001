//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdio.h>
#include <sys/wait.h>
#include "common.h"
#include "buffer.h"
#include "semaphore.h"
#include "iteration1.h"

void run_alternative1(int buffer_size, int number_of_producers, int number_of_consumers)
{
    printf("Running iteration1: producers=%d, consumers=%d, buffer=%d\n", number_of_producers, number_of_consumers, buffer_size);
    // Initialize semaphores
    // Notes:
    //  - S is for mutual exclusion of the buffer
    //  - N is to synchronize the current number of items in the buffer
    //  - E is to synchronzie the number of empty items
    sem_s = semaphore_create(SEMAPHORE_S, 1);
    sem_n = semaphore_create(SEMAPHORE_N, 0);
    sem_e = semaphore_create(SEMAPHORE_E, buffer_size);
    
    // Initialize shared buffer
    buffer_init(buffer_size);
    
    // Start producers
    start_children(1, number_of_producers, start_producer1);

    // Start consumers
    start_children(1, number_of_consumers, start_consumer1);
    
    // Wait for all children to terminate
    int status;
    int children_count = number_of_consumers + number_of_producers;
    for (int i = children_count; i > 0; i--) {
        wait(&status);
    }
    
    printf("Iteration 1 completed\n\n");
    
    return;
}

void start_producer1(int producer_id)
{
    printf("Starting producer: iteration=%d, id=%d\n", 1, producer_id);
    
    int value = generate_producer_value(producer_id);
    printf("Producer #%d: putting %d into buffer\n", producer_id, value);
    buffer_add(value);
}

void start_consumer1(int consumer_id)
{
    printf("Starting consumer: iteration=%d, id=%d\n", 1, consumer_id);
    
    int value = buffer_retrieve();
    printf("Consumer #%d: retrieved %d from buffer\n", consumer_id, value);
}

