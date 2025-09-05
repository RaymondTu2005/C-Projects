////////////////////////////////////////////////////////////////////////////////
// Main File:        send_signal.c
// This File:        send_signal.c
// Other Files:      my_c_signal_handler
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

/* This function takes in user input and sends the cooresponding signal based on that user input to a specified PID
 *
 * Pre-conditions: Three arguments, one containing program name, one containing intended signal, and one containing the PID
 * argc: Number of arguments, should be 3
 * argv: Contains executable name, intended operation, and PID in that order
 * retval: Returns 1 if failed operation, 0 if correct.
 */
int main (int argc, char** argv) {
	if (argc != 3) { 
		printf("Usage: %s -u <pid> to send SIGUSR1\n       %s -i <pid> to send SIGINT\n",argv[0],argv[0]);
		exit(1);
	}
	// The -u indicates SIGUSR1 and -i indicates SIGINT
	// We want to use argv[1] for the operation and argv[2] for the PID
	if (strcmp(argv[1], "-u") != 0 && strcmp(argv[1], "-i") != 0) { 
		printf("Incorrect argument one\n");
		exit(1);
	}
	// Now, get the PID
	int pid = atoi(argv[2]); 
	if (pid <= 0) { 
		printf("Invalid Process ID\n");
		exit(1);
	}
	// Now, send the signal
	if (strcmp(argv[1], "-u") == 0) { 
		// Send a SIGUSR1 by calling kill which has signal number 10
		if (kill(pid,10) != 0)  { 
			printf("Error sending SIGUSR1 signal\n");
			exit(1);
		}
	} 
	else { // We know if not -u then -i, which is SIGINT, signal number 2
		if (kill(pid,2) != 0) { 
			printf("Error sending SIGINT signal\n");
			exit(1);
		}	
	}
	exit(0);
}		
