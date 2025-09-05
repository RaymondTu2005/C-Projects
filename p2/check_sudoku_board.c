////////////////////////////////////////////////////////////////////////////////
// Main File:        check_sudoku_board.c
// This File:        check_sudoku_board.c
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
// Online sources:   w3 schools calloc function guide: helped fix warning of accessing
//                   uninitialized variables in an array
//					 https://www.w3schools.com/c/ref_stdlib_calloc.php
//                   Interpreting valgrind output: 
//					 https://valgrind.org/docs/manual/quick-start.html#quick-start.interpret
//                   GeeksForGeeks - Helped fix a problem of file pointer not being able to
//					 read the first line twice, therefore causing get_board_size to read from
//					 the board itself. Described the rewind function
//                   - https://www.geeksforgeeks.org/g-fact-82/
// 
// AI chats:         N/A 
//////////////////////////// 80 columns wide ///////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Copyright 2021-25 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
// Shared with permission: Hina Mahmood (Spring 2025)
// We have provided comments and structure for this program to help you get 
// started. Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *DELIM = ",";  // commas ',' are a common delimiter character for data strings
/*
 * Returns 1 if and only if the 2D array of ints in board 
 * is in a valid Sudoku board state. Otherwise returns 0.
 *
  DOES NOT PRODUCES ANY PRINTED OUTPUT
 * 
 * A valid row or column contains only blanks or the digits 1-size, 
 * with no duplicate digits, where size is the value 1 to 9.
 * 
 * Note: This function requires only that each row and each column are valid.
 * 
 * board: heap allocated 2D array of integers 
 *  size: number of rows and columns in the board
 */
int valid_sudoku_board(int **board, int size) {
    // First, we are going to check the rows of the board and make sure there is no
	// repeated numbers in a row. Going to implement this using an array that kind of 
	// acts as a hashmap.
	int num_checker;
	int *row_checker = NULL;
	int *column_checker = NULL;
	for (int i = 0; i < size; i++) {
		// Allocate memory on heap for an array to check a row.
		row_checker = calloc(size, sizeof(int));
		if (row_checker == NULL) {
		      return(0); 
	     }

		for  (int j = 0; j < size; j++) {

			// Check if input is already in the row_checker, if not, fill it,
			// but if it exists already, we have a duplicate, so return 0
			num_checker = *(*(board+i) + j);
			if (num_checker != 0) {
				if (num_checker > size || (*(row_checker + num_checker - 1)) == num_checker) { // Then already used or too big, so return 0
						// Since we have to return, free memory before we return.
						free(row_checker);
						return 0;
			} else { // Space is not taken, so fill it
				*(row_checker + num_checker - 1) = num_checker;
			}	
			}
		}
		free(row_checker); // Make sure no memory leaks, and so that we can reinitialize it again.
	}
	// Now, we have to check the columns. Do the same thing, but change the indexing.
	for (int i = 0; i < size; i++) {
		column_checker = calloc(size, sizeof(int));
		if (column_checker == NULL) {
			return 0;
		}
		for (int j = 0; j < size; j++) {
			num_checker = *(*(board+j) + i); // Now, change the columns, not the rows and check accordingly
			if (num_checker != 0) {
				if (num_checker > size || (*(column_checker + num_checker - 1)) == num_checker) {
					// Found a duplicate or number outside of bounds. Return 0 but free memory first.
					free(column_checker);
					return 0;		
			} else {
				*(column_checker + num_checker - 1) = num_checker;
			}
			}
		}
		free(column_checker);
	}
	// If none of our check return 0 (Duplicate / Out of Bounds), than we should return 1 to indicate proper function.
	return 1;   
}    

/* COMPLETED (DO NOT EDIT)       
 * Read the first line of file to get the size of the board.
 * 
 * PRE-CONDITION #1: file exists
 * PRE-CONDITION #2: first line of file contains valid non-zero integer value
 *
 * fptr: file pointer for the board's input file
 * size: a pointer to an int to store the size
 *
 * POST-CONDITION: the integer whose address is passed in as size (int *) 
 * will now have the size (number of rows and cols) of the board being checked.
 */
void get_board_size(FILE *fptr, int *size) {      
	char *line = NULL;
	size_t len = 0;

	// 'man getline' to learn about <stdio.h> getline
	if ( getline(&line, &len, fptr) == -1 ) {
		printf("Error reading the input file.\n");
		free(line);
		exit(1);
	}

	char *size_chars = NULL;
	size_chars = strtok(line, DELIM); // 'man strtok' string tokenizer
	*size = atoi(size_chars);         // 'man atoi' alpha to integer
	free(line);
}


/*
 * This program prints "valid" (without quotes) if the input file contains
 * a valid state of a Sudoku puzzle board wrt to rows and columns only.
 * It prints "invalid" (without quotes) if the input file is not valid.
 *
 * Usage: A single CLA that is the name of a file that contains data.
 *
 * argc: the number of command line args (CLAs)
 * argv: the CLA strings, includes the program name
 *
 * Returns 0 if file exists and is readable.
 * Exit with any non-zero result if unable to open and read a given file.
 */
int main( int argc, char **argv ) {              
	
	if (argc != 2) {
		printf("Usage: %s <input_filename>\n", *(argv+0));
		exit(1);
	}
	// Open the file 
	FILE *fp = fopen(*(argv + 1), "r");
	if (fp == NULL) {
		printf("Can't open file for reading.\n");
		exit(1);
		}
	int size;
	// Check if there is an integer value for size first
	// Before calling get_board_size(); 
	char *first_line = malloc(sizeof(char) * 15);
	if (first_line == NULL) { 
		printf("Error allocating dynamic memory");
		exit(1);
		}
	// Read the first 15 bytes worth of characters
	if (fgets(first_line, 15, fp) != NULL) {
		int value = atoi(first_line);
			// Atoi returns 0 if it does not work properly.
			if (value == 0) {	
				printf("invalid\n");
				free(first_line);
				if (fclose(fp) != 0) {
					printf("Error closing the file");
					exit(1);
				}
				return(0);
				} else if (value >= 1 || value <= 9) {
					// First, we have to reset the file pointer, so that when we call
					// get_board_size, it will read the first line. We can do this with
					// the rewind function.
					rewind(fp);
					get_board_size(fp, &size);
					free(first_line);
				} else {
					printf("invalid\n");
					free(first_line);
					if (fclose(fp) != 0) {
		                 printf("Error closing the file");
	                     exit(1);
					}
					return(0);
				}
	} else {
		printf("Cannot read input file\n");
		free(first_line);
		exit(1);
	}
	// Double check size after calling get_board_size
	if (size > 9 || size < 1) { 
		printf("invalid\n");
		if (fclose(fp) != 0) {
			printf("Error closing the file");
			exit(1);
		}	
		return 0;
	}
	// Creates 2D Array Pointer
	int **array_pointer;
	// Create the rows 
	array_pointer = malloc(size * sizeof(int *));
	if (array_pointer == NULL) {
		printf("Error allocating dynamic memory");
		exit(1); 
	}
	// Create the columns and assign it to pointers contained inside array_pointer
	for (int i = 0; i < size; i++) {
		*(array_pointer + i) = malloc(size * sizeof(int));
		if (*(array_pointer + i) == NULL) {
			printf("Error allocating dynamic memory");
			exit(1);
		}
	}
	// Read the remaining lines of the board data file.
	// Tokenize each line and store the values in your 2D array.
	char *line = NULL;
	size_t len = 0;
	char *token = NULL;
	for (int i = 0; i < size; i++) {

		// read the line
		if (getline(&line, &len, fp) == -1) {
			printf("Error while reading line %i of the file.\n", i+2);
			exit(1);
		}
		token = strtok(line, DELIM);
		for (int j = 0; j < size; j++) {
			*(*(array_pointer + i) + j) = atoi(token); 
			token = strtok(NULL, DELIM);
		}
	}

	int validBoard = valid_sudoku_board(array_pointer, size);
	
	if (validBoard == 1) {
		printf("valid\n");
	} else { // Can either be 1 or 0
		printf("invalid\n");
	}

	// Must use free size + 1 times to make sure we have no memory leaks, in reverse order of allocation
	for (int i = 0; i < size; i++) {
		free(*(array_pointer + i)); // Free Pointer to Array containing Integers
	}
	free(array_pointer); // Free the 2D Array Pointer
	free(line);
	//Close the file.
	if (fclose(fp) != 0) {
		printf("Error while closing the file.\n");
		exit(1);
	} 

	return 0;       
}       


