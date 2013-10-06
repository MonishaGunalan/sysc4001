//
//  SYSC 4001 - Assignment 1
//  Monisha Gunalan (100871444)
//  Korey Conway (100838924)
//  October 2013
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include "monitor.h"
#include <ctype.h>


#define MAX_TEXT 512

struct my_msg_st {
    long int msg_type;
    char patientName[MAX_TEXT];
    int heartbeat_rate;
	int pid;
};

￼￼￼￼￼￼￼￼struct parent_data {
    pid_t monitor_pid;
    //yet to fill in
    char some_data[SIZE];
};

int main(int argc, const char * argv[])
{
    return 0;
}

void parent(){
    int controller_fifo_fd, monitor_fifo_fd;
    int read_res;
    struct data_to_pass_st data;
    char monitor_fifo[256];
 
    
    
    // Make controller FIFO
    mkfifo(CONTROLLER_FIFO_NAME, 0777);
    
    // Open FIFO to read message from Monitor
    controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_RDONLY);
    if (controller_fifo_fd == -1) {
		fprintf(stderr, "Controller fifo failure\n");
		exit(EXIT_FAILURE);
	}
    
    // Read the message received from the Monitor
    do {
		read_res = read(controller_fifo_fd, &parent_data, sizeof(parent_data));
		if (read_res > 0) {
			printf("Message from Monitor: %s", parent_data.some_data);

			sprintf(monitor_fifo, MONITOR_FIFO_NAME, parent_data.monitor_pid);
            
			// Open FIFO to send message to Monitor
            monitor_fifo_fd = open(monitor_fifo, O_WRONLY);
			if (monitor_fifo_fd != -1) {
				write(monitor_fifo_fd, &parent_data, sizeof(parent_data));
				close(monitor_fifo_fd);
			}
		}
	} while (read_res > 0);
    
    close(controller_fifo_fd);
	unlink(CONTROLLER_FIFO_NAME);
	exit(EXIT_SUCCESS);
    
}



void start_child(struct fifo_data *data) {

    
    int running = 1;
    struct my_msg_st some_data;
    int msgid;
    char buffer[BUFSIZ];
    long int msg_type = 0;
    
    // Set up the message queue
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    
    while(running) {
        // Wait to receive message from monitor (Message Queue)
        if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                   msg_type, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        
        // Prints info on console (PID, patient name, heartbeat)
        printf("PID : %d \nPatient Name: %s \nHeart Beat Rate: %d", some_data.pid, some_data.patientName, some_data.heartbeat_rate);
        
        
        // Responds back with an ACK to monitor (Message Queue)
        some_data.patientName = "Ack";
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
        
    }
    
    end_child();
	
}



void end_child() {
	// if we are child
	// close ourselves
	// if we are parent
	// send signal to child to close
    
    
    // Delete message queue when end message is encountered
    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
    
}

