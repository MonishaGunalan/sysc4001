//
//  semaphore.h
//  SYSC 4001 Assignment 2
//
//  Created by Korey Conway on 11/6/2013.
//  Copyright (c) 2013 SG•Nexus. All rights reserved.
//

#ifndef SYSC_4001_Assignment_2_semaphore_h
#define SYSC_4001_Assignment_2_semaphore_h

#include <semaphore.h> // needed for sem_t type

sem_t * semaphore_create(char* name, int inital_value);
void semaphore_signal(sem_t* sem);
void semaphore_wait(sem_t* sem);
void semaphore_close(sem_t* sem);
void semaphore_delete(char* name);

#endif
