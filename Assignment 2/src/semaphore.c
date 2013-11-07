//
//  semaphore.c
//  SYSC 4001 Assignment 2
//
//  Created by Korey Conway on 11/6/2013.
//  Copyright (c) 2013 SG•Nexus. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>
#include "semaphore.h"

sem_t * semaphore_create(char* name, int initial_value)
{
    semaphore_delete(name);
    sem_t* sem = sem_open(name, O_CREAT, 0777, initial_value);
    if (SEM_FAILED == sem) {
        printf("Failed to create semaphore with errno=%d\n", errno);
        exit(0);
    }
    return sem;
}

void semaphore_signal(sem_t* sem)
{
    sem_post(sem);
}

void semaphore_wait(sem_t* sem)
{
    sem_wait(sem);
}

void semaphore_close(sem_t* sem)
{
    sem_close(sem);
}

void semaphore_delete(char* name)
{
    sem_unlink(name);
}

