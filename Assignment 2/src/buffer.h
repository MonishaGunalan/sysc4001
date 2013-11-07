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

typedef struct buffer_st {
    int in_index;
    int out_index;
    int values[BUFFER_MAX_VALUES];
} buffer_st;

void buffer_init(int size);
int  buffer_retrieve();
void buffer_add(int value);

#endif
