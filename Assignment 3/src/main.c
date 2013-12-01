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
char* priorityLevelToString(int priority_level);

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
	if ( 0 != pthread_mutex_init(&mutex, NULL) ) {
		printf("Failed to create mutex\n");
		exit(EXIT_FAILURE);
	}

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
		if ( 0 != pthread_join(threads[i], &thread_result) ) {
			printf("Failed to wait for thread: %d\n", i);
		}
	}
	printf("All threads finished\n");
	
	// Cleanup
	if ( 0 != pthread_mutex_destroy(&mutex) ) {
		printf("Failed to release mutex. No worries, we are ending anyways.\n");
	}
	
	exit(EXIT_SUCCESS);
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
				   && (queue_is_empy(&queue_bronze) || silver_count < MAX_SILVER_BEFORE_BRONZE)) {
			silver_count++;
			queue_dequeue(&queue_silver, &t);
		} else if (!queue_is_empy(&queue_bronze)) {
			silver_count = 0;
			queue_dequeue(&queue_bronze, &t);
		} else {
			// We are donce, so release lock and allocated memory
			pthread_mutex_unlock(&mutex);
			free(id_ptr);
			return NULL;
		}

		// Release the lock
		pthread_mutex_unlock(&mutex);

		// Process task
		int process_time;
		queue_t* queue;
		switch(t.priority_level) {
			case PRIORITY_LEVEL_GOLD:
				queue = &queue_gold;
				process_time = t.remaining_time;
				break;
			case PRIORITY_LEVEL_SILVER:
				queue = &queue_silver;
				if (t.remaining_time > MAX_PROCESS_TIME_SILVER) {
					process_time = MAX_PROCESS_TIME_SILVER;
				} else {
					process_time = t.remaining_time;
				}
				break;
			case PRIORITY_LEVEL_BRONZE:
			default:
				queue = &queue_bronze;
				if (t.remaining_time > MAX_PROCESS_TIME_BRONZE) {
					process_time = MAX_PROCESS_TIME_BRONZE;
				} else {
					process_time = t.remaining_time;
				}
				break;
		}
		printf("Thread %d: process_time=%ims, task={id=%i, priority=%s, remaining=%ims} \n", id, process_time, t.id, priorityLevelToString(t.priority_level), t.remaining_time);
		usleep(process_time * 1000);
		
		// Put back in queue if not finished
		if (t.remaining_time > process_time) {
			t.remaining_time -= process_time;
			pthread_mutex_lock(&mutex);
			queue_enqueue(queue, t);
			pthread_mutex_unlock(&mutex);
		}

	}
}

char* priorityLevelToString(int priority_level) {
	switch (priority_level) {
		case PRIORITY_LEVEL_GOLD:
			return "gold";
		case PRIORITY_LEVEL_SILVER:
			return "silver";
		case PRIORITY_LEVEL_BRONZE:
		default:
			return "bronze";
	}
}