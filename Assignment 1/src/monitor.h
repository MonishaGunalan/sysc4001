//
// SYSC 4001 - Assignment 1
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// October 2013
//

#ifndef monitor_h
#define monitor_h

// Constants
#define HEARTBEAT_INTERVAL 2
#define HEARTBEAT_MAX 100
#define HEARTBEAT_MIN 60
#define RAND_SEED (unsigned) time(NULL)
#define NAME_MAX_LENGTH 50
#define CONTROLLER_MESSAGE_QUEUE_KEY 5555
#define MONITOR_MESSAGE_QUEUE_KEY 4444

// Functions
void parent_loop(void);
void parent_cleanup(void);
bool child_setup(void);
void child_loop(void);
void child_cleanup(void);

// Variables
static char patient_name[NAME_MAX_LENGTH];
static int controller_queue_id = -1;
static int monitor_queue_id = -1;
static controller_msg msg_to_send;
static monitor_msg received_msg;
#endif
