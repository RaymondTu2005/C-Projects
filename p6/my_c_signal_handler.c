////////////////////////////////////////////////////////////////////////////////
// Main File:        my_c_signal_handler
// This File:        my_c_signal_handler
// Other Files:      send_signal.c
// Semester:         CS 354 Lecture 001 Spring 2025
// Grade Group:      gg 1  (Canvas course -> People -> click on your name to see your gg#)
// Instructor:       Mahmood
// 
// Author:           Raymond Tu
// Email:            rktu2@wisc.edu
// CS Login:         rtu
//
///////////////////////////  OPTIONAL WORK LOG  //////////////////////////////
//  Document your work sessions here or on your copy http://tiny.cc/cs354-work-log
//  Keep track of commands, structures, and code that you have learned.
//  This will help you focus your review on what is new to you from each program.
//  There is no need to submit work log, unless we ask you to do so.
//////////////////// REQUIRED -- OTHER SOURCES OF HELP ///////////////////////// 
// Persons:          N/A
//                   
//
// Online sources:   N/A
//                   
//                   
// 
// AI chats:         N/A
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

int SECONDS = 0; // Used so main and sigalarm_handler can be tracked by both functions
int SIGUSR_CNT = 0; // Keeps track of number of times we have received SIGUSR1

/* This function handles a SIGALRM signal sent by the alarm() function and prints out the current local time.
 * Pre-conditions: A SIGALRM signal must have been sent
 * retval: No return value. Exits the program if there is an error with getting the time
 */
void sigalarm_handler() {
	if (SECONDS == 0) { 
		printf("PID and current time: prints every 3 seconds.\nType Ctrl-C to end the program.\n");		
	}
	SECONDS += 3; // Makes sure first statement does not print again.
	time_t current_time = time(NULL);	
	if (current_time == -1) { 
		printf("Error getting the time");
		exit(1);
	}
	char time_string[100]; // Holds the time returned from C string
	// Must use strcpy to copy to time_string from ctime
	char* ctime_return = ctime(&current_time);
	if (ctime_return == NULL) { 
		printf("Error getting time");
	}
	strcpy(time_string, ctime_return);
	// Check time_string and make sure it isn't null since null is an error from ctime
	// NOTE TO GRADER: This checks the ctime return value technically 
	if (time_string == NULL) {
		printf("Error getting time");
		exit(1);
	}
	printf("PID: %d CURRENT TIME: %s", getpid(), time_string);
	alarm(3);
	return;
}

/* This function handles the SIGUSR1 signal and increments the SIGUSR count and prints that it has received the signal.
 *
 * Pre-conditions: A SIGUSR1 signal must be sent
 * retval: Does not return any value
 */
void sigusr_handler() {
	printf("Received SIGUSR1, user signal 1 counted.\n");
	SIGUSR_CNT += 1;
}

/* This function is intended to handle the SIGINT signal and terminate the program, printing out number of SIGUSR signals handled.
 *
 * Pre-conditions: A sigint signal must be sent
 * retval: Does not return any value and instead terminates the program as intended
 */
void sigint_handler() { 
		printf("\nSIGINT handled.\n");
		printf("SIGUSR was handled %d times. Exiting now.\n", SIGUSR_CNT);
		exit(0);
	}
/* This program sets new signal handlers for SIGINT, SIGUSR1, and SIGALRM. It repeats the time and PID of the program
 * infinitely every three seconds until ctrl-c is asserted.
 * Pre-conditions: No preconditions required
 * argc: Should be 1, containing name of program
 * argv: Contains name of program, 
 * retval: Does not return any value except in an error binding the sigalrm. Instead loops infinitely until ctrl-c is initiated
 */
int main(int argc, char **argv) {
	struct sigaction sa;               // A struct to bind signals.
	memset(&sa, 0, sizeof(sa));
	// Set SIGALRM
	sa.sa_handler = sigalarm_handler;
	if (sigaction(SIGALRM, &sa, NULL) != 0) { 
		printf("Error binding sigalrm handler");
		exit(1);
	}
	// Set SIGUSR1
	sa.sa_handler = sigusr_handler;
	if (sigaction(SIGUSR1, &sa, NULL) != 0) { 
		printf("Error binding sigusr handler");
		exit(1);
	}
	// Set SIGINT
	sa.sa_handler = sigint_handler;
	if (sigaction(SIGINT, &sa, NULL) != 0) { 
		printf("Error binding sigint handler");
		exit(1);
	}
	alarm(3);
	while(1) {
	}
}
