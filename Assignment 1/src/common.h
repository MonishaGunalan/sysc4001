//
//  SYSC 4001 - Assignment 1
//  Monisha Gunalan (100871444)
//  Korey Conway (100838924)
//  October 2013
//

#ifndef Assignment_1_common_h
#define Assignment_1_common_h

// Public types
typedef struct controller_msg {
    long int msg_key;
    int heartbeat;
} controller_msg;

typedef struct monitor_msg {
    long int msg_key;
    bool ack;
} monitor_msg;

typedef struct process_t {
    bool(*setup)();
    void(*loop)();
    void(*cleanup)();
} process_t;


// Private variables
static pid_t child_pid = -1;
static bool running = false;


// Public Functions
void start_processes(process_t parent, process_t child);
void send_sigterm_to_parent(void);
void dump(const char * format, ...);


// Private Functions
static void setup_signal_handling(void);
static void handle_signal(int sigid);
static bool is_parent();
static void end_main_loop(void);
static void run_process(process_t process);


#endif
