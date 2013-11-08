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
#include "alternative1.h"
#include <errno.h>
#include <unistd.h>

static sem_t* sem_s;
static sem_t* sem_n;
static sem_t* sem_e;

// Run the first alternative in the assignment
void run_alternative1(int buffer_size, int number_of_producers, int number_of_consumers)
{
    printf("Running alternative 1: producers=%d, consumers=%d, buffer=%d\n", number_of_producers, number_of_consumers, buffer_size);
    // Initialize semaphores
    // Notes:
    //  - S is for mutual exclusion of the buffer
    //  - N is to synchronize the current number of items in the buffer
    //  - E is to synchronzie the number of empty items
    sem_s = semaphore_create(SEMAPHORE_S, 1);
    sem_n = semaphore_create(SEMAPHORE_N, 0);
    sem_e = semaphore_create(SEMAPHORE_E, buffer_size);
    
    // Initialize shared buffer
    buffer_init(buffer_size, number_of_producers);
    
    // Start timer to measure performance
    timer_start();
    
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
    
    // Stop timer and display result
    double t = timer_stop();
    printf("\n----------------");
    printf("Iteration 1 with buffer_size=%d completed in %f \n\n", buffer_size, t);
    
    // Delete semaphores and shared memory
    semaphore_delete(SEMAPHORE_S);
    semaphore_delete(SEMAPHORE_N);
    semaphore_delete(SEMAPHORE_E);
    buffer_delete();
    
    return;
}

// Run the producer code
void start_producer1(int producer_id)
{
    verbose("Producer %d: starting", producer_id);
    
    for(int i = 0; i < ENTRIES_PER_PRODUCER; i++) {
        // Wait until there is room in the buffer
        verbose("Producer %d: waiting for room in buffer", producer_id);
        semaphore_wait(sem_e);
        
        // Wait until we have the buffer lock (S)
        verbose("Producer %d: waiting for buffer lock", producer_id);
        semaphore_wait(sem_s);
        
        // Add value into buffer
        int value = generate_producer_value(producer_id);
        int index = buffer_add(value);
        verbose("Producer %d: putting %d into buffer at index=%d", producer_id, value, index);
        
        // Pause
        time_pause();
        
        // Signal sempahores
        semaphore_signal(sem_n); // add 1 to count of elements in buffer
        semaphore_signal(sem_s); // release lock on buffer
    }

    // Signal to consumer to finish
    semaphore_wait(sem_e);
    semaphore_wait(sem_s);
//    int value = -1;
//    verbose("Producer %d: putting %d into buffer", producer_id, value);
//    buffer_add(value);
    buffer_decrement_producers();
    semaphore_signal(sem_n); // add 1 to count of elements in buffer
    semaphore_signal(sem_s); // release lock on buffer

    
    // Close semaphores and buffer for this process
    semaphore_close(sem_s);
    semaphore_close(sem_n);
    semaphore_close(sem_e);
    buffer_close();
    
    verbose("Producer %d finished", producer_id);
}



// Run the consumer code
void start_consumer1(int consumer_id)
{
    verbose("Starting consumer: iteration=%d, id=%d", 1, consumer_id);
    int value;
    int running = 1;
    
    do {
        // Wait until there is something in the buffer to consume
        verbose("Consumer %d: waiting for product in buffer", consumer_id);
        semaphore_wait(sem_n);

        // Wait until we have the buffer lock
        verbose("Consumer %d: waiting for buffer lock", consumer_id);
        semaphore_wait(sem_s);

        // Retrieve an item
        int index = buffer_retrieve(&value);
        if (index >= 0) {
            verbose("Consumer %d: retrieving %d from buffer at index=%d", consumer_id, value, index);
        } else {
            // Stop consumer
            running = 0;
            
            // Signal other consumers to stop by simulating an entry to read
            semaphore_signal(sem_n);
        }

        // Pause
        time_pause();
        
        // Signal sempahores
        semaphore_signal(sem_e); // add 1 to count of spaces in buffer
        semaphore_signal(sem_s); // release lock on buffer
    } while (running);
    
    // Close semaphores and buffer for this process
    semaphore_close(sem_s);
    semaphore_close(sem_n);
    semaphore_close(sem_e);
    buffer_close();
    
    verbose("Consumer %d finished", consumer_id);
}

