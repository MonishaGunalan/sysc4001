//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

pid_t fork_child(int alternative, int id, void (*callback)(int))
{
    int child_pid = fork();
    switch(child_pid) {
        case -1: // Error
            printf("Failed to fork child, alternative=%d, id=%d\n", alternative, id);
            exit(-1);  // stop from returning to "parent" code
        case 0: // Child
            srand(RAND_SEED + id);
            callback(id);
            exit(0); // stop from returning to "parent" code
            break;
        default: // Parent
            break;
    }
    
    return child_pid;
}

void start_children(int alternative, int child_count, void (*callback)(int))
{
    for (int i = 1; i <= child_count; i++) {
        fork_child(alternative, i, callback);
    }
}

int generate_producer_value(int producer_id)
{
    return producer_id * 1000 + (rand() % 100);
}
