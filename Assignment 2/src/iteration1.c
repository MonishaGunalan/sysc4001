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
#include "timer.h"
#include "iteration1.h"
#include <errno.h>

static sem_t* sem_s;
static sem_t* sem_n;
static sem_t* sem_e;

// Run the first alternative in the assignment
void run_alternative1(int buffer_size, int number_of_producers, int number_of_consumers)
{
    timer_start();
    
    printf("Running iteration1: producers=%d, consumers=%d, buffer=%d\n", number_of_producers, number_of_consumers, buffer_size);
    // Initialize semaphores
    // Notes:
    //  - S is for mutual exclusion of the buffer
    //  - N is to synchronize the current number of items in the buffer
    //  - E is to synchronzie the number of empty items
    sem_s = semaphore_create(SEMAPHORE_S, 1);
    sem_n = semaphore_create(SEMAPHORE_N, 0);
    sem_e = semaphore_create(SEMAPHORE_E, buffer_size);
    
    int y;
    int x = sem_getvalue(sem_n, &y);
    printf("Sem e count: %d\n", y);
    printf("Sem e count: %d\n", x);
    printf("Sem e count: %d\n", errno);
    exit(0);

    
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
    
    double t = timer_stop();
    printf("Iteration 1 completed in %f         \n\n", t);
    
    // Delete semaphores and shared memory
    semaphore_delete(SEMAPHORE_S);
    semaphore_delete(SEMAPHORE_N);
    semaphore_delete(SEMAPHORE_E);
    buffer_close();
    
    return;
}

// Run the producer code
void start_producer1(int producer_id)
{
    printf("Producer %d: starting\n", producer_id);
    
    for(int i = 0; i < ENTRIES_PER_PRODUCER; i++) {
        // Wait until there is room in the buffer
        printf("Producer %d: waiting for room in buffer\n", producer_id);
        semaphore_wait(sem_e);
        
        // Wait until we have the buffer lock (S)
        printf("Producer %d: waiting for buffer lock\n", producer_id);
        semaphore_wait(sem_s);
        
        int value = generate_producer_value(producer_id);
        printf("Producer #%d: putting %d into buffer\n", producer_id, value);
        buffer_add(value);
    }
}

// Run the consumer code
void start_consumer1(int consumer_id)
{
    printf("Starting consumer: iteration=%d, id=%d\n", 1, consumer_id);
    int value;
    
    do {
        // Wait until there is something in the buffer to consume
        printf("Consumer %d: waiting for product in buffer\n", consumer_id);
        semaphore_wait(sem_n);

        // Wait until we have the buffer lock
        printf("Consumer %d: waiting for buffer lock\n", consumer_id);
        semaphore_wait(sem_s);

        // Retrieve an item
        value = buffer_retrieve();
        printf("Consumer #%d: retrieved %d from buffer\n", consumer_id, value);
    } while (value != -1);
}

