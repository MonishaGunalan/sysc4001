//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdio.h>
#include "main.h"
#include "buffer.h"
#include "iteration1.h"

void start_producer1(int producer_id)
{
    printf("Starting producer: iteration=%d, id=%d\n", 1, producer_id);
}

void start_consumer1(int consumer_id)
{
    printf("Starting consumer: iteration=%d, id=%d\n", 1, consumer_id);
}

void run_alternative1(int buffer_size, int number_of_producers, int number_of_consumers)
{
    // Initialize semaphores
    // TODO
    
    // Initialize shared buffer
    buffer_init(buffer_size);
    
    // Start producers
    for (int i = 1; i <= number_of_producers; i++) {
        if ( fork_producer(1, i) <= 0 ) {
            // Return when it is just the child returning or error
            return;
        }
    }
    
    // Start consumers
    for (int i = 1; i <= number_of_consumers; i++) {
        if ( fork_consumer(1, i) <= 0 ) {
            // Return when it is just the child returning or error
            return;
        }
    }
    
    return;
}
