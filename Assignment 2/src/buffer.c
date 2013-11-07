//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include "buffer.h"

static int buffer_size;
static int buffer_memory_size;
static int buffer_fd; // Buffer file descriptor
static buffer_st* buffer;

// Initializes the buffer in shared memory
void buffer_init(int buff_size)
{
    // Save buffer size in global var
    buffer_size = (buff_size > BUFFER_MAX_VALUES) ? BUFFER_MAX_VALUES : buff_size;
    buffer_memory_size = sizeof(buffer_st);
    
    // Create shared memory
    buffer_fd = shm_open(BUFFER_SHM_KEY, O_RDWR | O_CREAT, 0666);
    
    // Check for errors
    if (-1 == buffer_fd) {
        fprintf(stderr, "Failed to create buffer\n");
        exit(-1);
    }
    
    // Allocate the buffer memory size
    ftruncate(buffer_fd, buffer_memory_size);
    
    // Map the shared memory to the process
    buffer = (buffer_st *) mmap(NULL, buffer_memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, buffer_fd, 0);
    
    // Initialize the indices
    buffer->in_index = 0;
    buffer->out_index = 0;
}

// Retrieves the next value from the buffer and increments the out index
int  buffer_retrieve()
{
    int value = buffer->values[buffer->out_index];
    buffer->out_index = (buffer->out_index + 1) % buffer_size;
    msync(buffer, buffer_memory_size, MS_SYNC | MS_INVALIDATE);
    return value;
    return 5;
}

// Adds a value the the buffer and increments the in index
void buffer_add(int value)
{
    buffer->values[buffer->in_index] = value;
    buffer->in_index = (buffer->in_index + 1) % buffer_size;
    msync(buffer, buffer_memory_size, MS_SYNC | MS_INVALIDATE);
}

// Closes the buffer
void buffer_close()
{
    close(buffer_fd);
}

// Deletes the buffer
void buffer_delete()
{
    shm_unlink(BUFFER_SHM_KEY);
}
