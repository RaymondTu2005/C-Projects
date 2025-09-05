//////////////////////////////////////////////////////////////////////////////
// Main File:        csim.c
// This File:        csim.c
// Other Files:      (name of all other files, if any)
// Semester:         CS 354 Lecture 00? Spring 2025
// Grade Group:      gg 01  (Canvas course -> People -> click on your name to see your gg#)
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
////////////////////////////////////////////////////////////////////////////////
// Copyright 2013,2019-2025
// Posting or sharing this file is prohibited, including any changes/additions.
// Used with permission for Spring 2025
////////////////////////////////////////////////////////////////////////////////

/**
 * csim.c:  
 * Simulate the contents of a cache with given configuration and 
 * count the number of hits, misses, and evictions for a given 
 * sequence of memory accesses for a program.
 *
 * If you want to create your own memory access traces, you can 
 * use valgrind to output traces that this simulator can analyze.
 *
 * Which replacement policy is implemented?  LRU (Least-Recently Used) Replacement Policy
 *
 * Implementation and assumptions:
 *  1. (L) load or (S) store cause at most one cache miss and a possible eviction.
 *  2. (I) Instruction loads are ignored.
 *  3. (M) Data modify is treated as a load followed by a store to the same
 *     address. Hence, an (M) operation can result in two cache hits, 
 *     or a miss and a hit plus a possible eviction.
 *
 * Find the TODO tags to see where to make chanes for your simulator.
 */  

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/******************************************************************************/
/* DO NOT MODIFY THESE VARIABLE NAMES and TYPES                               */
/* DO UPDATE THEIR VALUES AS NEEDED BY YOUR CSIM                              */

//Globals set by command line args.
int b = 0; //number of (b) bits
int s = 0; //number of (s) bits
int E = 0; //number of lines per set

//Globals derived from command line args.
int B; //block size in bytes: B = 2^b
int S; //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0;
int miss_cnt = 0;
int evict_cnt = 0;

//Global to control trace output
int verbosity = 0; //print trace if set
/******************************************************************************/


// Type mem_addr_t: stores addresses or address masks.
typedef unsigned long long int mem_addr_t;

// Type cache_line_t: stores "overhead" (v-bit and tag) for a cache line
// The cache block's data is not needed or stored for this simulator.
typedef struct cache_line {                    
    char valid;
    mem_addr_t tag;
	int counter;
	// Going to be using a counter to find the least recently used cache line
} cache_line_t;

// Type cache_set_t: Stores a pointer to the first cache line in a set.
// Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t;

// Type cache_t: Stores a pointer to the first set in a cache
// Note: The cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t;

// Create the cache we are simulating. 
// Note: The cache is a pointer to a heap array of one or more sets.
cache_t cache;  

/*
 * init_cache:
 * Allocates the data structure for a cache with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */                    
void init_cache() {    
	// We should initialize data structures using malloc to hold information about the set and cache lines.
	// We will initialize it depending on the number of sets and lines per set.
	// First, update the S bit.
	S = 1 << s;
	// Now, we should initialize a cache
	cache = malloc(((S * E * sizeof(cache_line_t))));
	if (cache == NULL) { 
		printf("Error allocating dynamic memory on the heap");
		exit(0);
	}
	// Next, iterate through the cache and set all the V-bits and T-Bits to 0.
	for (int i = 0; i < S; i++) {
		void* setIterator = (void*) cache + (sizeof(cache_line_t) * E) * i;
		for (int j = 0; j < E; j++) {
			void* lineIterator = setIterator + (sizeof(cache_line_t) * j);
			cache_line_t* line_modifier = (cache_line_t*) lineIterator;
			line_modifier->valid = 0;
			line_modifier->tag = 0;
			line_modifier->counter = 0;
		}
	}
}

/*
 * free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() {    
	free(cache);
	cache = NULL; // Prevent it from being used again and causing an error.         
}


/*
 * access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * If block containing addr is already in cache, increment hit_cnt
 * If block containing addr is not in cache, 
 *    cache it (set tag and valid), increment miss_cnt
 * If a block in a cache line is evicted, increment evict_cnt
 */					
void access_data(mem_addr_t addr) {      
	B = 1 << b;
	// We should check the set tag to check if the block is in the cache already.
	// We do this first by finding the set number. First, left shift the set number until the LSB contains one of the set bits
	// Then, mask the address to find the set number.
	mem_addr_t right_shift_addr = addr >> b;
	// Now, we calculate the masking address
	mem_addr_t set_mask = 0;
	for (int i = 0; i < s; i++) { 
		set_mask += 1 << i;
	}
	int set_number = set_mask & right_shift_addr;
	// Now, we should get the tag bits.
	mem_addr_t t_bit_getter = addr >> (b + s);
	mem_addr_t t_mask = 0;
	t_mask = (1 << (64 - b - s)) - 1;
	mem_addr_t tag_addr = t_bit_getter & t_mask;
	// Now that we have our set number, we can check the set to see if the addr is contained in any of the lines.
	// We do this by matching the tag number and seeing if they match. If they do, we have a hit
	for (int i = 0; i < E; i++) {
		void* line_iterator = ((void*) cache + sizeof(cache_line_t) * set_number * E) + (sizeof(cache_line_t) * i);
		cache_line_t* line_checker = (cache_line_t*) line_iterator;		
		// Now, we check if any of the tag bits match. If it is, that means it is stored in memory and is a hit.
		if (line_checker->valid == 1 && line_checker->tag == tag_addr) {
			// Therefore, it is valid and the tag bits match so increment hit_cnt and update the counter
			hit_cnt += 1;
			int highest_counter = -1;
			for (int j = 0; j < E; j++) { 
				void* line_counter = ((void*) cache + sizeof(cache_line_t) * set_number * E) + (sizeof(cache_line_t) * j);
				cache_line_t* high_counter = (cache_line_t*) line_counter;
				if (high_counter->counter > highest_counter) highest_counter = high_counter->counter;
			}
			// If the current line is the highest counter, keep it the same, but if it isn't, update it to 1 + highest counter
			line_checker->counter = (highest_counter == line_checker->counter) ? highest_counter : highest_counter + 1;
			return;
		}
	}
	// After the loop, we have not hit. So we have missed, and we either have to fill in a spot, or evict it from something else.
	int min_counter = 2147483647;
	int high_counter = -1;
	void* setIterator = (void*) cache + (sizeof(cache_line_t) * E) * set_number;
	cache_line_t* LRU = (cache_line_t*) setIterator;
		for (int j = 0; j < E; j++) { 
			void* lineIterator = setIterator + (sizeof(cache_line_t) * j);
			cache_line_t* checkLRU = (cache_line_t*) lineIterator;
			// If the set is not full, fill the first line with size 0.
			if (checkLRU->counter > high_counter) high_counter = checkLRU->counter;
			if (checkLRU->counter == 0) {
				// We should fill this spot.
				miss_cnt += 1;
				checkLRU->counter = high_counter + 1;
				checkLRU->valid = 1;
				checkLRU->tag = tag_addr;
				return;			
			} else if (checkLRU->counter < min_counter) {	
				min_counter = checkLRU->counter;
				LRU = checkLRU;
				}
			}
	// After the loop, if we have not found a counter = 0, we must evict something.
	evict_cnt += 1;
	miss_cnt += 1;
	LRU->counter = high_counter + 1;
	LRU->tag = tag_addr;
}


/* 
 * replay_trace:
 * Replays the given trace file against the cache.

 Example: subset of trace, shows type of access, address, size of access

 L 7ff0005b8,4
 S 7feff03ac,4
 M 7fefe059c,4
 
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE "L" as a Load i.e. one memory access
 * TRANSLATE "S" as a Store i.e. one memory access
 * TRANSLATE "M" as a Modify which is a load followed by a store, 2 mem accesses 
 */                    
void replay_trace(char* trace_fn) {           
    char buf[1000];  
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE* trace_fp = fopen(trace_fn, "r"); 

    if (!trace_fp) { 
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);   
    }

    while (fgets(buf, 1000, trace_fp) != NULL) { 
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);

            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);

			if (buf[1] == 'S') {
				access_data(addr);		
			} else if (buf[1] == 'L') {
				access_data(addr);
			} else {
				access_data(addr);
				access_data(addr);
			}
			
            if (verbosity)
                printf("\n");
        }
    }

    fclose(trace_fp);
}  


/*
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Verbose flag.\n");
    printf("  -s <num>   Number of s bits for set index.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of b bits for word and byte offsets.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}  


/*
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}  


/*
 * main:
 * parses command line args, 
 * makes the cache, 
 * replays the memory accesses, 
 * frees the cache and 
 * prints the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
    char* trace_file = NULL;
    char c;

    // Parse the command line arguments: -h, -v, -s, -E, -b, -t 
    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 'b':
                b = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'h':
                print_usage(argv);
                exit(0);
            case 's':
                s = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            default:
                print_usage(argv);
                exit(1);
        }
    }

    // Make sure that all required command line args were specified.
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage(argv);
        exit(1);
    }

    // Initialize cache.
    init_cache();

    // Replay the memory access trace.
    replay_trace(trace_file);

    // Free memory allocated for cache.
    free_cache();

    // Print statistics to a file.
    // DO NOT REMOVE: This function must be called for test_csim to work.
    print_summary(hit_cnt, miss_cnt, evict_cnt);
    return 0;   
}  

//			END OF CODE (SPRING 2025)                                     

