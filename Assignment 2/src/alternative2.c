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
#include "alternative2.h"
#include <errno.h>
#include <unistd.h>

#define NUMBER_OF_PRODUCERS 1
#define NUMBER_OF_CONSUMERS 1

static sem_t* sem_n;
static sem_t* sem_e;

// Run the first alternative in the assignment
void run_alternative2(int buffer_size)
{
    printf("Running alternative 2: producers=%d, consumers=%d, buffer=%d\n", NUMBER_OF_PRODUCERS, NUMBER_OF_CONSUMERS, buffer_size);
    // Initialize semaphores
    // Notes:
    //  - N is to synchronize the current number of items in the buffer
    //  - E is to synchronzie the number of empty items
    sem_n = semaphore_create(SEMAPHORE_N, 0);
    sem_e = semaphore_create(SEMAPHORE_E, buffer_size);
    
    // Initialize shared buffer
    buffer_init(buffer_size, NUMBER_OF_PRODUCERS);
    
    // Start timer to measure performance
    timer_start();
    
    // Start producers
    start_children(2, NUMBER_OF_PRODUCERS, start_producer2);
    
    // Start consumers
    start_children(2, NUMBER_OF_CONSUMERS, start_consumer2);
    
    // Wait for all children to terminate
    int status;
    int children_count = NUMBER_OF_CONSUMERS + NUMBER_OF_PRODUCERS;
    for (int i = children_count; i > 0; i--) {
        wait(&status);
    }
    
    // Stop timer and display result
    double t = timer_stop();
    printf("Alternative 2 completed in %f\n", t);
    
    // Delete semaphores and shared memory
    semaphore_delete(SEMAPHORE_N);
    semaphore_delete(SEMAPHORE_E);
    buffer_delete();
    
    return;
}

// Run the producer code
void start_producer2(int producer_id)
{
    verbose("Producer %d: starting", producer_id);
    
    for(int i = 0; i < ENTRIES_PER_PRODUCER; i++) {
        // Wait until there is room in the buffer
        verbose("Producer %d: waiting for room in buffer", producer_id);
        semaphore_wait(sem_e);
        
        // Add value into buffer
        int value = generate_producer_value(producer_id);
        int index = buffer_add(value);
        verbose("Producer %d: putting %d into buffer at index=%d", producer_id, value, index);

        // Sleep
        usleep(SLEEP_UTIME);
        
        // Signal sempahores
        semaphore_signal(sem_n); // add 1 to count of elements in buffer
    }
    
    // Signal to consumer to finish
    semaphore_wait(sem_e);

    int value = -1;
    verbose("Producer %d: putting %d into buffer", producer_id, value);
    buffer_add(value);
    semaphore_signal(sem_n); // add 1 to count of elements in buffer
  
    
    // Close semaphores and buffer for this process
    semaphore_close(sem_n);
    semaphore_close(sem_e);
    buffer_close();
}

// Run the consumer code
void start_consumer2(int consumer_id)
{
    verbose("Starting consumer: alternative=%d, id=%d", 1, consumer_id);
    int value;
    int running = 1;
    
    do {
        // Wait until there is something in the buffer to consume
        verbose("Consumer %d: waiting for product in buffer", consumer_id);
        semaphore_wait(sem_n);
        
        int index = buffer_retrieve(&value);
        if (index >= 0) {
            verbose("Consumer %d: retrieving %d from buffer at index=%d", consumer_id, value, index);
        } else {
            // Stop consumer
            running = 0;
            
            // Signal other consumers to stop by simulating an entry to read
            semaphore_signal(sem_n);
        }
        
        // Signal sempahores
        semaphore_signal(sem_e); // add 1 to count of spaces in buffer
    } while (running);
    
    // Close semaphores and buffer for this process
    semaphore_close(sem_n);
    semaphore_close(sem_e);
    buffer_close();
}

