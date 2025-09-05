////////////////////////////////////////////////////////////////////////////////
// Main File:        sequence.c
// This File:        sequence.c
// Other Files:      (name of all other files, if any)
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
void calculateValue(int num_times, int start, int multiply, int addee, char* final_string) {
    for (int i = 0; i < num_times; i++) {
         if (i == (num_times - 1)) {
               // Must do + strlen because it will keep overriding if not and not adding it to the end.
               // If last number, should not add a comma to end
               sprintf(final_string + strlen(final_string), "%d\n", start);
           } else {
               sprintf(final_string + strlen(final_string), "%d,", start);
           }
		start *= multiply;
		start += addee;
     }
}
 /* A program that generates a sequence using y=mx+b based on user input
 *
 * Pre-conditions: Must have 4 CLA Arguments denoting number of values, multiplier, multiplee, and starting value
 * retval: This function returns 0 if it successfully reaches the end without any errors.
 */
int main(int argc, char** argv) {
	if (argc != 5) { 
		printf("Usage: %s n x0 m c\n",*(argv+0));
		exit(0);
	}
	int number_of_values = atoi(*(argv+1));
	int starting_value = atoi(*(argv+2));
	int multiplier = atoi(*(argv+3));
	int adder = atoi(*(argv+4));
	char output[1000] = ""; // Initialize, noting we can only modify with strcpy
	calculateValue(number_of_values, starting_value, multiplier, adder, output);
	printf("%s",output);
}

