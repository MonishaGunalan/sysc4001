//
//  controller.h
//  Assignment 1
//
//  Created by Korey Conway on 2013-10-07.
//  Copyright (c) 2013 SG•Nexus. All rights reserved.
//

#ifndef Assignment_1_controller_h
#define Assignment_1_controller_h

#define MAX_MONITORS 25

// Functions
bool parent_init(void);
void parent_loop(void);
void parent_cleanup(void);
void close_monitor(int monitor_pid);

bool child_setup(void);
void child_loop(void);
void child_cleanup(void);

// Variables
int controller_fifo_fd = -1;
int msg_queue_id = -1;
int monitor_pid[MAX_MONITORS];
int monitor_count = 0;

#endif
