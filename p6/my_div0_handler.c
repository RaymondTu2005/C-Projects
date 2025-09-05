////////////////////////////////////////////////////////////////////////////////
// Main File:        my_div0_handler.c
// This File:        my_div0_handler.c
// Other Files:      N/As
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

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
int number_of_ops = 0; // Tracks number of division operations

/* A function that handles division by 0 errors and prints out the total number of operations before termination.
 *
 * Pre-conditions: A sigfpe signal must be sent
 * retval: Exits the program and returns the value 0.
 */
void sigfpe_handler() { 
	printf("Error: a division by 0 operation was attempted.\n");
	printf("Total number of operations completed successfully: %d\n", number_of_ops);
	printf("The program will be terminated.\n");
	exit(0);	
}
/* A function that handles ctrl-c (SIGINT) signals and prints the total number of operations before termination.
 *
 * Pre-conditions: A SIGINT signal must be sent
 * retval: Exits the program and returns the value 0.
 */
void sigint_handler() { 
	printf("\nTotal number of operations completed successfully: %d\n", number_of_ops);
	printf("The program will be terminated\n");
	exit(0);
}
/* This function binds signals to functions and prompts user for input. Computes division algorithm with quotient and remainder and prints it out.
 *
 * Pre-conditions: The user inputs are not null
 * argc: Unused
 * argv: Unused
 * retval: Loops forever, returning 1 if an error occurs (Other functions returns 0 and terminates)
 */
int main(int argc, char** argv) { 
    struct sigaction sa;                // A struct to bind signals to functions
	memset(&sa, 0, sizeof(sa));
	// Set SIGFPE
	sa.sa_handler = sigfpe_handler;
	if (sigaction(SIGFPE, &sa, NULL) != 0) {
		printf("Error binding sigfpe handler");
        exit(1);
    }
    sa.sa_handler = sigint_handler;
	if (sigaction(SIGINT, &sa, NULL) != 0) {
		printf("Error binding sigint handler");
		exit(1);
	}
	while (1) { 
	printf("Enter first integer: "); 
	char first_int[100];
	if (fgets(first_int, 100, stdin) == NULL) { 
		printf("Error getting user input");
		exit(1);
	}
	// Since no error, now call atoi.
	int first_integer = atoi(first_int);
	// Now repeat
	printf("Enter second integer: ");
	char second_int[100];
	if (fgets(second_int, 100, stdin) == NULL) { 
		printf("Error getting user input");
		exit(1);
	}
	int second_integer = atoi(second_int);
	// To get remainder, we have to convert the integers to doubles.
	int quotient = first_integer / second_integer;
	double remainder =  first_integer - (quotient * second_integer);
	printf("%d / %d is %d with a remainder of %d\n", first_integer, second_integer, quotient, (int) remainder);
	number_of_ops += 1;
	}
}
