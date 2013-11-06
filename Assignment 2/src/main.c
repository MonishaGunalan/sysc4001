//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"
#include "iteration1.h"

int main(int argc, const char * argv[])
{
    srand(RAND_SEED);
    run_alternative1(50, 3, 5);
    return 0;
}

int generate_producer_value(int producer_id)
{
    return producer_id * 1000 + (rand() % 100);
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

pid_t fork_consumer(int iteration, int consumer_id)
{
    int child_pid = fork();
    switch(child_pid) {
        case -1: // Error
            printf("Failed to fork consumer, iteration=%d, id=%d\n", iteration, consumer_id);
            break;
        case 0: // Child
            switch (iteration) {
                case 1:
                    start_consumer1(consumer_id);
                    break;
                default:
                    printf("Unkown iteration. Cannot start consumer\n");
                    break;
            }
            break;
        default: // Parent
            break;
    }
    
    return child_pid;
}