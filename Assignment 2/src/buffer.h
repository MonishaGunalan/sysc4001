//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#ifndef SYSC_4001_Assignment_2_buffer_h
#define SYSC_4001_Assignment_2_buffer_h

#define BUFFER_SHM_KEY "/assign2_buffer"
#define BUFFER_MAX_VALUES 500
#define BUFFER_SMALL    2
#define BUFFER_MEDIUM   5
#define BUFFER_LARGE   50

typedef struct buffer_st {
    int in_index;
    int out_index;
    int producers; // count of number of producers
    int values[BUFFER_MAX_VALUES];
} buffer_st;

void buffer_init(int size, int producers_count);
int  buffer_retrieve(int* value);
int  buffer_add(int value);
void buffer_close(void);
void buffer_delete(void);
void buffer_decrement_producers(void);

#endif
