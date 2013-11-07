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
    
    printf("Running alternative2: producers=%d, consumers=%d, buffer=%d\n", NUMBER_OF_PRODUCERS, NUMBER_OF_CONSUMERS, buffer_size);
    // Initialize semaphores
    // Notes:
    //  - N is to synchronize the current number of items in the buffer
    //  - E is to synchronzie the number of empty items
   
    sem_n = semaphore_create(SEMAPHORE_N, 0);
    sem_e = semaphore_create(SEMAPHORE_E, buffer_size);
    
    // Initialize shared buffer
    buffer_init(buffer_size);
    
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
    
    double t = timer_stop();
    printf("Alternative2 completed in %f         \n\n", t);
    
    // Delete semaphores and shared memory
    semaphore_delete(SEMAPHORE_N);
    semaphore_delete(SEMAPHORE_E);
    buffer_close();
    
    return;
}

// Run the producer code
void start_producer2(int producer_id)
{
    printf("Producer %d: starting\n", producer_id);
    
    for(int i = 0; i < ENTRIES_PER_PRODUCER; i++) {
        // Wait until there is room in the buffer
        printf("Producer %d: waiting for room in buffer\n", producer_id);
        semaphore_wait(sem_e);
        
        // Add value into buffer
        int value = generate_producer_value(producer_id);
        printf("Producer %d: putting %d into buffer\n", producer_id, value);
        buffer_add(value);
        
        // Sleep
        usleep(SLEEP_UTIME);
        
        // Signal sempahores
        semaphore_signal(sem_n); // add 1 to count of elements in buffer
    }
    
    // Signal to consumer to finish
    semaphore_wait(sem_e);

    int value = -1;
    printf("\nProducer %d: putting %d into buffer\n\n", producer_id, value);
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
    printf("Starting consumer: alternative=%d, id=%d\n", 1, consumer_id);
    int value;
    
    do {
        // Wait until there is something in the buffer to consume
        printf("Consumer %d: waiting for product in buffer\n", consumer_id);
        semaphore_wait(sem_n);
        
        // Retrieve an item
        value = buffer_retrieve();
        printf("Consumer %d: retrieved %d from buffer\n", consumer_id, value);
        
        // Signal sempahores
        semaphore_signal(sem_e); // add 1 to count of spaces in buffer
    } while (value != -1);
    
    // Close semaphores and buffer for this process

    semaphore_close(sem_n);
    semaphore_close(sem_e);
    buffer_close();
}

