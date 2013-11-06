//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#ifndef SYSC_4001_Assignment_2_buffer_h
#define SYSC_4001_Assignment_2_buffer_h

int buffer_in_index = 0;
int buffer_out_index = 0;
int buffer_size = 0;

void buffer_init(int size);
int  buffer_get_size(void);
int  buffer_get_value(int index);
void buffer_set_value(int index, int value);

#endif
