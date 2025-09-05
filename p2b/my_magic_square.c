//////////////////////////////////////////////////////////////////////////////
// Main File:		 my_magic_square.c
// This File:        my_magic_square.c
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
// Online sources:   Stack Overflow: https://stackoverflow.com/questions/65693813/does-using-malloc-to-create-a-string-not-automatically-add-a-null-terminator
//                   Helped fix an issue where fprintf was printing random characters at a newline
//                   because the uninitialized heap memory did not contain a null terminator.
// 
// AI chats:         Save a transcript and submit with your project.
//////////////////////////// 80 columns wide ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2025, Hina Mahmood
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
	int size;           // dimension of the square
	int **magic_square; // ptr to 2D heap array that stores magic square values
} MagicSquare;

/* 
 * Prompts the user for magic square's size, read size, and
 * check if it is an odd number >= 3 
 * If not valid size, display the required error message and exit
 *
 * return the valid number
 */
int getSize() {
	int size_of_square;
    printf("Enter magic square's size (odd integer >=3)\n");
	scanf("%d", &size_of_square);
	// Size of 0 or less indicates EOF or failure to match.
	if ((size_of_square % 2) == 0) {
		printf("Magic square size must be odd.\n");
		return 0;
	} else if (size_of_square < 3) {
		printf("Magic square size must be >= 3.\n");
		return 0;
	} 
	return size_of_square;   
} 

/* 
 * Creates a magic square of size n on the heap
 *
 * May use the Siamese magic square algorithm or alternative
 * algorithm that produces a valid magic square 
 *
 * n - the number of rows and columns
 *
 * returns a pointer to the completed MagicSquare struct
 */
MagicSquare *generateMagicSquare(int n) {
	// First, we dynamically allocate the MagicSquare struct.
	MagicSquare *output_square = malloc(sizeof(MagicSquare));
	if (output_square == NULL) {
		printf("Failed to allocate dynamic memory for the MagicSquare Struct!\n");
		exit(1);
	}
	output_square->size = n;
	// Allocate an N by N square now.
	output_square->magic_square = malloc(sizeof(int * ) * output_square->size);
	if (output_square->magic_square == NULL) {
		printf("Error allocating dynamic memory for the MagicSquare Array\n");
		exit(1);
	}
	// Create N columns that are initialized to 0
	for (int i = 0; i < output_square->size; i++) {
		(*((output_square->magic_square) + i)) = calloc(output_square->size, sizeof(int));
		if ((*((output_square->magic_square) + i)) == NULL) {
			printf("Error allocating dynamic memory for MagicSquare Columns\n");
			exit(1);
         }
	}
	// Start at column (n/2), row 0
	int row = 0;
	int column = (n/2);
	int temp_row;
	int temp_column;
	int placed_num = 0; // A 0 indicates placed, 1 indicates not placed.
	// A lot easier when first square is already filled, so fill it.
	(*(*((output_square->magic_square) + row) + column)) = 1;
	// Now that we have allocated the space, start filling the square.
	// If we have a N sized square, we will fill N^2 - 1 elements since the first is already filled
	for (int i = 2; i < (n * n + 1); i++) {
		placed_num = 1;
		// Using Siamese Method
		// First check row - 1 column + 1 square.
		while(placed_num != 0) { 
			temp_row = ((row - 1) == -1) ? (n-1) : (row -1);
			temp_column = ((column + 1) == n) ? 0 : (column + 1);		
			if ((*(*((output_square->magic_square) + temp_row) + temp_column)) == 0) { 
				// Fill it.
				(*(*((output_square->magic_square) + temp_row) + temp_column)) = i;
				row = temp_row;
				column = temp_column;
				placed_num = 0;
				continue;
			} else { // Square that is up one and to the right is occupied, so check the one below.	
				temp_row = ((row + 1) == n) ? 0 : (row + 1);
				temp_column = column;
				if ((*(*((output_square->magic_square) + temp_row) + temp_column)) == 0) { 
					// The square is NOT filled, so fill it.
					(*(*((output_square->magic_square) + temp_row) + temp_column)) = i;
					row = temp_row;
					column = temp_column;
					placed_num = 0;
					continue;
				}
			}
		// We have not found an empty square, so set the row to row -1 and loop again.
		row = temp_row;
		column = temp_column;
		// The loop should automatically continue and loop until we fill a square with the number.
		}
	}
	return output_square;    
} 

/*   
 * Open a new file (or overwrite the existing file)
 * and write magic square values to the file
 * in a format specified in the assignment.
 *
 *
 * magic_square - the magic square to write to a file
 * filename - the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	// Format should be size as the first line.
	// Following lines should be number at square, followed by comma with no spaces.
	FILE *fp = fopen(filename,"w");
	if (fp == NULL) { 
		printf("Error opening file.\n");
		exit(1);
	}
	char *first_line = malloc(sizeof(char) * 10);
	if (first_line == NULL) {
		printf("Error allocating memory for a string\n");
		if (fclose(fp) != 0) {
			printf("Error closing the file");
		}
		exit(1);
	}
	// First line is the size
	sprintf(first_line, "%d\n", magic_square->size);
	fputs(first_line,fp);
	free(first_line);
	
	// Now, add the Magic Square
	for (int i = 0; i < magic_square->size; i++) { // Rows
		char* row_to_add = malloc(sizeof(char) * 6 * magic_square->size); // Ensure enough storage allocated
		*(row_to_add + 0) = '\0'; // Uninitialized, so add a null terminator so sprintf will add one too, or else undefined behavior.
		if (row_to_add == NULL) { 
			printf("Error allocating dynamic memory for string\n");
			if (fclose(fp) != 0) {
		       printf("Error closing the file");
			}
			exit(1);
		}	
		for (int j = 0; j < magic_square->size; j++) {
			if (j != ((magic_square->size) - 1)) { 
				// Append with comma
				// We do row_to_add + strlen(row_to_add) so the numbers don't override.)
				sprintf(row_to_add + strlen(row_to_add), "%d,",(*(*((magic_square->magic_square) + i) + j)));
			} else {
				// Append without comma since it is the last line
				// We do row_to_add + strlen(row_to_add) so the numbers don't override.)
				sprintf(row_to_add + strlen(row_to_add), "%d",(*(*((magic_square->magic_square) + i) + j)));
			}
		}
		// Now add the line and then free memory accordingly.
		fprintf(fp, "%s\n", row_to_add);
		free(row_to_add);
	}
	if (fclose(fp) != 0) { 
		printf("Error closing file\n");
		exit(1);
	}
}

/* 
 * Calls other functions to generate a magic square 
 * of the user-specified size and outputs the
 * created square to the output filename.
 * 
 * argv - contains name of the file to be written to
 * returns 1 if unable to get size and write to file, 0 if successful
 */
int main(int argc, char **argv) {
	// Check input arguments to get output filename
	if (argc != 2) { 
		printf("Usage: %s <output_filename>\n",*(argv+0));
		exit(1);
	}

	int size_of_square = getSize();
	if (size_of_square == 0) { // Indicates failure
		return 1;
	}
	MagicSquare *generated_square = generateMagicSquare(size_of_square);	
	// Output the magic square
	// Should now call the File Output Function
	fileOutputMagicSquare(generated_square, *(argv+1));
	// Now, free up the magic square, in reverse order of allocation to prevent memory loss.
	for (int i = 0; i < generated_square->size; i++) { 
		free((*((generated_square->magic_square) + i)));
	}	
	free(generated_square->magic_square);
	free(generated_square);
	return 0;
} 

// 202501


