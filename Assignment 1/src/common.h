//
//  SYSC 4001 - Assignment 1
//  Monisha Gunalan (100871444)
//  Korey Conway (100838924)
//  October 2013
//

#ifndef Assignment_1_common_h
#define Assignment_1_common_h

// Constants
#define CONTROLLER_FIFO_NAME "./fifo.controller"
#define MONITOR_FIFO_NAME "./fifo.monitor.%d"
#define MONITOR_FIFO_NAME_MAX_LENGTH sizeof(MONITOR_FIFO_NAME) + 5
#define MESSAGE_QUEUE_KEY 5555
#define NAME_MAX_LENGTH 50

// Public types
typedef struct process_t {
	bool(*setup)();
	void(*loop)();
	void(*cleanup)();
} process_t;

typedef struct msg_data {
	long int destination_key;
	struct {
        int source_key;
		int heartbeat;
		bool ack;
		char patient_name[NAME_MAX_LENGTH];
	} data;
} msg_data;

typedef struct fifo_data {
	pid_t source_pid;
	enum {
		START, // controller tells monitor to start
		STOP, // controller tells monitor to stop
		CONNECT, // monitor requests connection to controller
		STOPPING // monitor informs controller that it is stopping
	} request_type;
} fifo_data;


// Public variables
static pid_t child_pid = -1;

// Private variables
static bool running = false;


// Public Functions
void start_fork(process_t parent, process_t child);
void send_sigterm_to_parent(void);
void dump(const char * format, ...);
int get_child_pid(void);


// Private Functions
static void setup_signal_handling(void);
static void handle_signal(int sigid);
static bool is_parent();
static void end_loop(void);
static void run_process(process_t process);


#endif
