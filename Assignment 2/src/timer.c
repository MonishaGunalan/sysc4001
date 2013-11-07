//
// SYSC 4001 - Assignment 2
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// November 2013
//

#include <time.h>
#include "timer.h"

static double timer_total_time = 0;
static time_t timer_start_time = 0;

// Start timer
void timer_start()
{
    timer_start_time = time(NULL);
    timer_total_time = 0;
}

// Pause timer
void timer_pause()
{
    timer_total_time += difftime(time(NULL), timer_start_time);
}

// Continue timer
void timer_continue()
{
    timer_start_time = time(NULL);
}

// Stop timer
double timer_stop()
{
    timer_pause();
    double total_time = timer_total_time;
    timer_total_time = 0;
    timer_start_time = 0;
    return total_time;
}
