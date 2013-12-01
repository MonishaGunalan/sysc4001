//
// SYSC 4001 - Assignment 3
// Monisha Gunalan (100871444)
// Korey Conway (100838924)
// December 2013
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "queue.h"
#include "common.h"

void create_task(void);
void* process_tasks(void* id_ptr);

queue_t queue_gold;
queue_t queue_silver;
queue_t queue_bronze;
pthread_t threads[THREAD_COUNT];
pthread_mutex_t mutex;

int silver_count = 0;


int main(int argc, const char * argv[])
{
	// Seed random numbers
	srand(getpid());
	
	// Initialize queues
	queue_init(&queue_gold);
	queue_init(&queue_silver);
	queue_init(&queue_bronze);
	
	// Create tasks
	for (int i = 0; i < TASK_COUNT; i++) {
		create_task();
	}
	
	// Create mutex
	pthread_mutex_init(&mutex, NULL);

	// Start threads
	for (int i = 0; i < THREAD_COUNT; i++) {
		int* thread_id = malloc(sizeof(thread_id));
		*thread_id = i;
		if ( pthread_create(&threads[i], NULL, process_tasks, thread_id) ) {
			printf("Failed to create thread %d\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	// Wait for threads to finish
	void *thread_result;
	for (int i = 0; i < THREAD_COUNT; i++) {
		pthread_join(threads[i], &thread_result);
	}
	printf("All threads finished\n");
	
	// Cleanup
	pthread_mutex_destroy(&mutex);
}

void create_task()
{
	static int id = 0;
	task_t t;
	int priority;
	int priority_level;
	int execution_time;
	
	// Determine priority and execution time
	priority = rand() % PRIORITY_MAX;
	if (priority < PRIORITY_LEVEL_GOLD) {
		priority_level = PRIORITY_LEVEL_GOLD;
		execution_time = (rand() % (TIME_GOLD_MAX - TIME_GOLD_MIN)) + TIME_GOLD_MIN;
	} else if (priority < PRIORITY_LEVEL_SILVER) {
		priority_level = PRIORITY_LEVEL_SILVER;
		execution_time = (rand() % (TIME_SILVER_MAX - TIME_SILVER_MIN)) + TIME_SILVER_MIN;
	} else {
		priority_level = PRIORITY_LEVEL_BRONZE;
		execution_time = (rand() % (TIME_BRONZE_MAX - TIME_BRONZE_MIN)) + TIME_BRONZE_MIN;
	}
	
	// Round execution time to 10ms granularity
	execution_time -= execution_time % 10;
	
	// Create the task structure
	t.id = id++;
	t.priority = priority;
	t.priority_level = priority_level;
	t.remaining_time = execution_time;
	t.total_time = execution_time;

	// Add to appropriate queue
	if (PRIORITY_LEVEL_GOLD == priority_level) {
		queue_enqueue(&queue_gold, t);
	} else if (PRIORITY_LEVEL_SILVER == priority_level) {
		queue_enqueue(&queue_silver, t);
	} else {
		queue_enqueue(&queue_bronze, t);
	}
}

void* process_tasks(void* id_ptr)
{
	int id = *((int *)id_ptr);
	printf("Thread started: %i\n", id);
	
	task_t t;
	
	while(1) {
		// Get the lock
		pthread_mutex_lock(&mutex);

		// Get task from queue
		if (!queue_is_empy(&queue_gold)) {
			// Check if there is a gold task
			queue_dequeue(&queue_gold, &t);
		} else if (!queue_is_empy(&queue_silver)
				   && (!queue_is_empy(&queue_bronze) || silver_count < MAX_SILVER_BEFORE_BRONZE)) {
			silver_count++;
			queue_dequeue(&queue_silver, &t);
		} else if (!queue_is_empy(&queue_bronze)) {
			silver_count = 0;
			queue_dequeue(&queue_bronze, &t);
		} else {
			// Release allocated memory and return
			free(id_ptr);
			return NULL;
		}

		// Release the lock
		pthread_mutex_unlock(&mutex);

		// Process task
		int process_time;
		switch(t.priority_level) {
			case PRIORITY_LEVEL_GOLD:
				process_time = t.remaining_time;
				break;
			case PRIORITY_LEVEL_SILVER:
				if (t.remaining_time > MAX_PROCESS_TIME_SILVER) {
					process_time = MAX_PROCESS_TIME_SILVER;
				} else {
					process_time = t.remaining_time;
				}
				break;
			case PRIORITY_LEVEL_BRONZE:
			default:
				if (t.remaining_time > MAX_PROCESS_TIME_BRONZE) {
					process_time = MAX_PROCESS_TIME_BRONZE;
				} else {
					process_time = t.remaining_time;
				}
				break;
		}
		usleep(process_time * 1000);
	}
	
}

