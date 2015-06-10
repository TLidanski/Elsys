//--------------------------------------------
// NAME: Tihomir Lidanski
// CLASS: XI A
// NUMBER: 27
// PROBLEM: #3
// FILE NAME: starcraft.c
// FILE PURPOSE:
// Implementing a prototype of Starcraft III
// 
// ...
//---------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
/* Variables */
int init_minerals = 5000;
int minerals_left = 5000;
int your_minerals = 0;

int miners = 5;
int soldiers = 0;
int Centers = 1;

pthread_mutex_t lock;

//--------------------------------------------
// FUNCTION: Mine
// This function does the mining work
// PARAMETERS:
// A void argument which is the number of the miner
// In the critical section we operate with the mineral variables
//----------------------------------------------
void *mine(void *arg) {
	int miner_num = *((int*) arg);


	/* As long as there are less than 20 soldiers or there are still minerals left (i.e. before the game ends) we collect minerals */
	if (soldiers < 20 && minerals_left >= 8) {

		printf("SCV %d is mining\n", miner_num);
		printf("SCV %d is transporting minerals\n", miner_num);	
				pthread_mutex_lock(&lock);
					minerals_left -= 8;       // Critical section
				pthread_mutex_unlock(&lock);

				sleep(2);

				pthread_mutex_lock(&lock);	
					your_minerals += 8;       // Critical section
				pthread_mutex_unlock(&lock);

				printf("SCV %d delivered minerals to Command center\n", miner_num);		
	
	}	else if (soldiers == 20 && minerals_left <= 7) { //If there are 20 soldiers or there aren't any minerals left we drop the threads
			pthread_exit(NULL);
	}

}

//--------------------------------------------
// FUNCTION: Main
// This the main function.It initializes the mutexes,creates the threads and handles the user input
// PARAMETERS:
// We read the user's input 
// Threads and mutexes are created and initialized in the function
//----------------------------------------------
int main() {
	/* Variables */
	int i;
	int handle;

	pthread_t thread[5];	

	char user_input = getchar();
	getchar();	
	while (soldiers < 20) {


	/* Handling the user input commands for creating troops and building command centers as long as there are less than 20 soldiers */
		switch(user_input) {
			case 's':
				if ((miners+soldiers < 200) && your_minerals >= 50) {
					pthread_mutex_lock(&lock);
					your_minerals -= 50;  // Critical section
					pthread_mutex_unlock(&lock);
					sleep(1);
					printf("good to go, sir\n");
					miners++;

				} else { printf("Not enough minerals\n"); }
			break;	


			case 'm':
				if ((miners+soldiers < 200) && your_minerals >= 50) {
					pthread_mutex_lock(&lock);
					your_minerals -= 50;       // Critical section
					pthread_mutex_unlock(&lock);
					sleep(1);
					printf("you wanna piece of me, boy\n");
					soldiers++;


				} else { printf("Not enough minerals\n"); }
			break;

			case 'c':
				if (your_minerals >= 400) {

					pthread_mutex_lock(&lock);
					your_minerals -= 400;    // Critical section
					pthread_mutex_unlock(&lock);
					sleep(2);
					printf("command center %d created\n", Centers);
					Centers++;

				} else { printf("Not enough minerals\n"); }
			break;	
			}	
		}	
	}

	/* Initializing the mutex */
	handle = pthread_mutex_init(&lock, NULL);
	if (handle != 0) {
		printf("errcode[%d]",handle);		
		abort();
	}	

	/* Creating threads for each miner */
	for (i=0;i<miners;i++) {
		handle = pthread_create(&thread[i], NULL, mine, NULL);
		if (handle != 0) {
			printf("errcode[%d]",handle);		
			abort();
		}	
	}


	/* Joining the threads */
	for (i=0;i<miners;i++) {
		handle = pthread_join(thread[i], NULL);
		if (handle != 0) {
			printf("errcode[%d]",handle);		
			abort();
		}	
	}

	pthread_mutex_destroy(&lock);   // Destroying the mutex

	printf("Initial minerals: %d\n Minerals left: %d\n Minerals you've mined: %d\n", init_minerals, minerals_left, your_minerals);
	return 0;
}
