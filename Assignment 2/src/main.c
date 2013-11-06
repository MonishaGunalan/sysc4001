//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, const char * argv[])
{
    srand(RAND_SEED);
    run_alternative1(50, 3, 1);
    return 0;
}

void run_alternative1(int buffer_size, int number_of_producers, int number_of_consumers)
{
    buffer_init(buffer_size);
    for (int i = 1; i <= number_of_producers; i++) {
        if ( fork_producer(1, i) <= 0 ) {
            // Break when it is just the child returning or error
            break;
        }
    }
    return;
}

pid_t fork_producer(int iteration, int producer_id)
{
    int child_pid = fork();
    switch(child_pid) {
        case -1: // Error
            printf("Failed to fork producer, iteration=%d, id=%d\n", iteration, producer_id);
            break;
        case 0: // Child
            switch (iteration) {
                case 1:
                    start_producer1(producer_id);
                    break;
                default:
                    printf("Unkown iteration. Cannot start producer\n");
                    break;
            }
            break;
        default: // Parent
            break;
    }
    
    return child_pid;
}

void start_producer1(int producer_id)
{
    printf("Starting producer: iteration=%d, id=%d\n", 1, producer_id);
}

void buffer_init(int buffer_size)
{
    return;
}

int generate_producer_value(int producer_id)
{
    return producer_id * 1000 + (rand() % 100);
}
