//
//  common.h
//  SYSC 4001 Assignment 2
//
//  Created by Korey Conway on 11/4/2013.
//  Copyright (c) 2013 SG•Nexus. All rights reserved.
//

#ifndef SYSC_4001_Assignment_2_common_h
#define SYSC_4001_Assignment_2_common_h

#define SEMAPHORE_S "/semaphore_s"
#define SEMAPHORE_N "/semaphore_n"
#define SEMAPHORE_E "/semaphore_e"

#define BUFFER_SIZE 5

int in_index = 0;
int out_index = 0;
int buffer[BUFFER_SIZE];

#endif
