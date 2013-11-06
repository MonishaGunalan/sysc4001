//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdio.h>

int schmid; // shared memory identifier
int *shared_memory_array; // Pointer to the first byte of shared memory


int  buffer_get_size(void)
{
    return 1;
}


void buffer_init(int buffer_size)
{
    
    //Create shared memory
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    
    //Make the shared memory accessible
    shared_memory_array = (int *)shmat(shmid, NULL, 0);
    
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", (int)shared_memory);
    return;
}


int  buffer_get_value(int index){
    int value = shared_memory_array[index];
    //********** check: need an indicator that the value has been removed and producer ca add a value to this index.
    //********** For now I have used 0
    shared_memory_array[index] = 0;
    return value;
}

void buffer_set_value(int index, int value){
    shared_memory_array[index] = value;
}

void detach_shared_memory(){
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
