////////////////////////////////////////////////////////////////////////////////
// Main File:        cla_sequence.c 
// This File:        cla_sequence.c
// Other Files:      N/A
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
#include <string.h>
#include <stdlib.h>
/* A program that generates a sequence based on user input. 
 *
 * Pre-conditions: Must have 2 user CLA arguments with the first number being smaller than the second.
 * void: This function does not require any command line arguments 
 * retval: This function returns 0 if it successfully reaches the end without any errors.
 */
int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Usage: %s start_integer end_integer where start is less than end", *(argv + 0));
		exit(1);
	}
	// Convert string into integer values
	int i = atoi(*(argv + 1));
	int j = atoi(*(argv + 2));
	if (i >= j) {
		printf("Usage: %s start_integer end_integer where start is less than end\n", *(argv + 0));
		exit(1);
	}	
	// Give enough room to store the string
	char print_string[1000] = "";
	for (int q = i; q < j + 1; q++) {
		if (q == j) { 		
			// Must do + strlen because it will keep overriding if not and not adding it to the end.
			// If last number, should not add a comma to end
			sprintf(print_string + strlen(print_string), "%d", q); 
		} else {
			sprintf(print_string + strlen(print_string), "%d", q);
			strcat(print_string, ",");
		}
	}

	printf("%s", print_string);

	return 0;
}
