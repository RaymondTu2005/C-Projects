////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Drows.c
// This File:		 cache2Drows.c
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
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
// 
// AI chats:         Save a transcript and submit with your project.
//////////////////////////// 80 columns wide ///////////////////////////////////

#define GLOBAL_ROW 3000
#define GLOBAL_COL 500

int two_dimens_array[GLOBAL_ROW][GLOBAL_COL];

int main() { 
	for (int i = 0; i < GLOBAL_ROW; i++) { 
		for (int j = 0; j < GLOBAL_COL; j++) { 
			two_dimens_array[i][j] = i + j;
		}
	}
	return 0;
}
