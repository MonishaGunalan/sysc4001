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

// Functions
bool parent_init(void);
void parent_loop(void);
void parent_cleanup(void);
bool child_setup(void);
void child_loop(void);
void child_cleanup(void);

// Variables
static char patient_name[NAME_MAX_LENGTH];
static int msg_queue_id = -1;
int controller_key;
char monitor_fifo_name[MONITOR_FIFO_NAME_MAX_LENGTH];


#endif
