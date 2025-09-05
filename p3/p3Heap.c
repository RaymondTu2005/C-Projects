////////////////////////////////////////////////////////////////////////////////
// Main File:        N/A
// This File:        p3Heap.c
// Other Files:      (name of all other files, if any)
// Semester:         CS 354 Lecture 001 Spring 2025
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
// Persons:          N/A
//                   
//
// Online sources:   N/A
//                   
//                   
//
// AI chats:         Save a transcript and submit with your project.
//////////////////////////// 80 columns wide ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2025 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes.
// Used with permission, SPRING 2025, Dr. Hina Mahmood
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2025 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes.
// Used with permission, SPRING 2025, Dr. Hina Mahmood
//
/////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "p3Heap.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block.
 */
typedef struct blockHeader {           

    /*
     * 1) The size of each heap block must be a multiple of 8
     * 2) Heap blocks have blockHeaders that contain size and status bits
     * 3) Free heap blocks contain a footer, but we can use the blockHeader 
     *
     * All heap blocks have a blockHeader with size and status
     * Free heap blocks have a blockHeader as its footer with size only
     *
     * Status is stored using the two least significant bits
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * Start Heap: 
     *  The blockHeader for the first block of heap is after skipping 4 bytes.
     *  This ensures alignment requirements can be met.
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
    int size_status;

} blockHeader;         

/* Global variable - DO NOT CHANGE NAME or TYPE. 
 * It must point to the first block in the heap and is set by init_heap()
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to the nearest page size.
 */
int alloc_size;




/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if size < 1 
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly add padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 *   Return NULL if unable to find and allocate block of required size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header. It is the address of the start of the 
 *       available memory for the requester.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* alloc(int size) {     
	if (size <= 0) {
		return NULL;
    }
    // Size will be size + 4 rounded to nearest multiple of 8. 
	// We run into trouble at size = 4, because it should not round up since 4 + 4 = 8, so check if size is a multiple
	// of 4, but NOT a multiple of 8.
    int block_size = (size % 4 == 0 && size % 8 != 0) ? size + 4 : size + 4 + (8 - ((size + 4) % 8));
    // We should check the headers now and compare them to find the best fit. If we find an exact match, stop
    // but if we don't continue until we find the "best fit". Continue until the end and if we reach it, go
    // back to the best fit.
	int best_fit = 2147483647; // If unchanged at end, there is no free block that fits, so return NULL;
	blockHeader* best_fit_header;
    void* currentLocation = (void*) heap_start;
	while (currentLocation < (void*) heap_start + alloc_size) {
		blockHeader* current_header = (blockHeader*) currentLocation;
		if ((current_header->size_status % 8) % 2 == 0) { 
			// It is not allocated, get the size
			int size_of_block = (current_header->size_status % 8 == 2) ? current_header->size_status - 2 : current_header->size_status;
			// Now, compare it to bestFit
			if (size_of_block == block_size) {
				// First, update the header and the next header if it exists. Then return a void* to the current location
				if (currentLocation + size_of_block < (void*) heap_start + alloc_size) { 
					// Then there is a next header. Update the next header and the current header, and return the payload.
					current_header->size_status += 1;
					blockHeader* next_header = (blockHeader*) (currentLocation + size_of_block);
					next_header->size_status += 2;
					void* return_alloc = currentLocation + sizeof(blockHeader*);
					return return_alloc;
				} else {
					// There is no next header, so update it and then return the payload
					current_header->size_status+= 1;
					void* return_alloc = currentLocation + sizeof(blockHeader*);
					return return_alloc;
				}
			// Block size is not an exact fit, so we continue on.
			} else if (size_of_block > block_size && size_of_block < best_fit) {
				best_fit = size_of_block;
				best_fit_header = current_header;
			}
			currentLocation = currentLocation + size_of_block;
			continue;
		} 
		// The current header is allocated, so move on to the next location.
		int size_of_alloc_block = (current_header->size_status % 8 == 3) ? current_header->size_status - 3 : current_header->size_status - 1;
		currentLocation = currentLocation + size_of_alloc_block;
	}
	// After we reach end and haven't found anything, check best fit. If value of best_fit is equal to the max value in an integer, return NULL since cannot fulfill request.
	if (best_fit == 2147483647 || best_fit < block_size) {
		return NULL;
	}
	// There exists a best fit block at best_fit_header, so fill that. We know it is > than block_size, so we have to split it.
	// First, update our best fit header to make it now taken and have the post-split size. Make sure to check previous block status.
	best_fit_header->size_status = (best_fit_header->size_status % 8 == 2) ? block_size + 3 : block_size + 1;
	// Create a new block header for the free block that we have and make sure its p-bit is 1.
	blockHeader* new_free_header = (blockHeader*) ((void*) best_fit_header + block_size);
	new_free_header->size_status = best_fit - block_size + 2;
	// Make the footer
	blockHeader* new_free_footer = (blockHeader*) ((void*) new_free_header + new_free_header->size_status - 6);
	new_free_footer->size_status = best_fit - block_size;
	void* return_payload = (void*) best_fit_header + sizeof(blockHeader*);
	return return_payload;
}
	

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 *
 * If free results in two or more adjacent free blocks,
 * they will be immediately coalesced into one larger free block.
 * so free blocks require a footer (blockHeader works) to store the size
 *
 * TIP: work on getting immediate coalescing to work after your code 
 *      can pass the tests in partA and partB of tests/ directory.
 *      Submit code that passes partA and partB to Canvas before continuing.
 */                    
int free_block(void *ptr) {    
    if (ptr == NULL || (int) ptr % 8 != 0 || ptr > ((void*)heap_start + alloc_size) || ptr < (void*) heap_start) { 
		// This checks if ptr is a multiple of 8, outside of heap space, or is NULL. If any are true, return -1
		return -1; 
	}
	// Now, we check the size header, making sure it is not already freed (p bit is 1 or number is odd)
	blockHeader* check_size = (blockHeader*) (ptr - ((void*) sizeof(blockHeader*))); 
	if (check_size->size_status % 2 == 0) { 
		return -1; // Block is currently freed.
	}
	// Now, we must change the size header and properly adjust the forward header's size.
	check_size->size_status -= 1;
	// Update the forward block's P-bit by minusing 2 if it is not the end of the heap.
	int size_of_block = (check_size->size_status % 8 == 2) ? (check_size->size_status - 2) : (check_size->size_status);
	blockHeader* nextLocation = (blockHeader*) (ptr + size_of_block - sizeof(blockHeader*));
	if ((void*) nextLocation < (void*) heap_start + alloc_size) {
		nextLocation->size_status -=2; // Update P-Bit to be freed
	}
	// Add a footer now.
	blockHeader* new_footer = (blockHeader*) (ptr + size_of_block - sizeof(blockHeader*) - sizeof(blockHeader*));
	new_footer->size_status = size_of_block;
	// Now, we implement immediate coalesing. In my opinion, easier to merge blocks ahead first then merge backwards.
	// We start at the block we just freed and check the block ahead.
	int size_of_current_block = size_of_block;
	blockHeader* start_block = check_size;
	blockHeader* next_block = (blockHeader*) (size_of_current_block + ptr - sizeof(blockHeader*));
	while ((void*) next_block < ((void*)heap_start + alloc_size) && next_block->size_status % 2 != 1) {
		blockHeader* remove_footer = (blockHeader*) ((void*) next_block - sizeof(blockHeader*));
		remove_footer->size_status = 0;
		// Now, we should update the next blocks' footer.
		int size_of_next_block = ((next_block->size_status) % 8 == 2) ? next_block->size_status - 2 : next_block->size_status;
		blockHeader* update_footer = (blockHeader*) ((void*) next_block + size_of_next_block - sizeof(blockHeader*));
		update_footer->size_status += size_of_current_block;
		next_block->size_status = 0;
		start_block->size_status += size_of_next_block;
		size_of_current_block += size_of_next_block;
		next_block = (blockHeader*)((void*) start_block + size_of_current_block);
	}
	// After this has finished, we should have coalesed with everything in front. Now we work backwards. Check if there is a footer, and if so, it is free.
	blockHeader* before_footer = (blockHeader*) ((void*) start_block - sizeof(blockHeader*));
	int size_of_previous_block;
	while (((void*) before_footer > (void*) heap_start) && before_footer->size_status != 0 && start_block->size_status % 8 != 2) {
		size_of_previous_block = before_footer->size_status;
		before_footer->size_status = 0;
		// We know that since there is a footer, it is not occupied, so merge it.
		// First update the footer at the end of the block.
		blockHeader* update_footer = (blockHeader*)((void*) start_block + size_of_current_block - sizeof(blockHeader*));
		update_footer->size_status += size_of_previous_block;
		// Now, we update the current start block to be the previous block's header and update that size accordingly.
		blockHeader* new_header_block = (blockHeader*) ((void*) start_block - size_of_previous_block);
		new_header_block->size_status += size_of_current_block;
		start_block->size_status = 0;
		start_block = new_header_block;
		size_of_current_block += size_of_previous_block;
		before_footer = (blockHeader*) ((void*) start_block - sizeof(blockHeader*));
	}
    return 0;
} 


/* 
 * Initializes the memory allocator.
 * Called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    

    static int allocated_once = 0; //prevent multiple myInit calls

    int   pagesize; // page size
    int   padsize;  // size of padding when heap size is not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int   fd;

    blockHeader* end_mark;

    if (0 != allocated_once) {
        fprintf(stderr, 
                "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize from O.S. 
    pagesize = getpagesize();

    // Calculate padsize, as padding is required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }

    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader*)((void*)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // Note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;

    return 0;
} 

/* STUDENTS MAY EDIT THIS FUNCTION, but do not change function header.
 * TIP: Review this implementation to see one way to traverse through
 *      the blocks in the heap.
 *
 * Can be used for DEBUGGING to help you visualize your heap structure.
 * It traverses heap blocks and prints info about each block found.
 * 
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     

    int    counter;
    char   status[6];
    char   p_status[6];
    char * t_begin = NULL;
    char * t_end   = NULL;
    int    t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size =  0;
    int free_size =  0;
    int is_used   = -1;

    fprintf(stdout, 
            "********************************** HEAP: Block List ****************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
            "--------------------------------------------------------------------------------\n");

    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;

        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
                p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
            "--------------------------------------------------------------------------------\n");
    fprintf(stdout, 
            "********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
            "********************************************************************************\n");
    fflush(stdout);

    return;  
} 


//		p3Heap.c		END OF FILE                    
                                       
