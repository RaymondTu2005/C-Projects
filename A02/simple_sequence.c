////////////////////////////////////////////////////////////////////////////////
// Main File:        simple_sequence.c 
// This File:        simple_sequence.c
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
// Online sources:   GeeksForGeeks String H Library Functions: 
//                   https://www.geeksforgeeks.org/c-library-string-h/
//                   GeeksForGeeks sprintf() in C:
//					 https://www.geeksforgeeks.org/sprintf-in-c/  
// 
// AI chats:         N/A
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <string.h>

/* A program that generates a sequence 1-10. 
 *
 * Pre-conditions: This program does not requre any pre-conditions to function properly
 * void: This function does not require any command line arguments 
 * retval: This function returns 0 if it successfully reaches the end without any errors.
 */
int main(void) {
	// This is the counter we will printout how many numbers we want
	int i;
	// Give enough room to store the string, although 30 characters may be excessive
	char print_string[30] = "";

	for (i = 1; i < 11; ++i) {
		if (i == 10) { 		
			// Must do + strlen because it will keep overriding if not and not adding it to the end.
			// If last number, should not add a comma to end
			sprintf(print_string + strlen(print_string), "%d", i); 
		} else {
			sprintf(print_string + strlen(print_string), "%d", i);
			strcat(print_string, ",");
		}
	}

	printf("%s", print_string);

	return 0;
}
