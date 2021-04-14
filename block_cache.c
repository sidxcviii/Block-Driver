////////////////////////////////////////////////////////////////////////////////
//
//  File           : block_cache.c
//  Description    : This is the implementation of the cache for the BLOCK
//                   driver.
//



// Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
// Project includes
#include <block_cache.h>
#include <block_driver.h>

uint32_t block_cache_max_items = DEFAULT_BLOCK_FRAME_CACHE_SIZE; // Maximum number of items in cache

// Cache block structure
struct cache_block {
	char data[BLOCK_FRAME_SIZE];
	int timeAccessed;
	int frameNum;
	int slot;
};	

typedef struct cache_block c_b;

int cache_size;

c_b *cb;

// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : set_block_cache_size
// Description  : Set the size of the cache (must be called before init)
//
// Inputs       : max_frames - the maximum number of items your cache can hold
// Outputs      : 0 if successful, -1 if failure

int set_block_cache_size(uint32_t max_frames)
{	
    cache_size = max_frames;	
    return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : init_block_cache
// Description  : Initialize the cache and note maximum frames
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int init_block_cache(void)
{
    
    cb = calloc(cache_size, BLOCK_FRAME_SIZE); 
   
    return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : close_block_cache
// Description  : Clear all of the contents of the cache, cleanup
//
// Inputs       : none
// Outputs      : o if successful, -1 if failure

int close_block_cache(void)
{
    if (cb != NULL) {	
    	//free(cb);
    	cb = NULL;
    }	
    return (0);
    
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : put_block_cache
// Description  : Put an object into the frame cache
//
// Inputs       : block - the block number of the frame to cache
//                frm - the frame number of the frame to cache
//                buf - the buffer to insert into the cache
// Outputs      : 0 if successful, -1 if failure

int put_block_cache(BlockIndex block, BlockFrameIndex frm, void* buf)
{
    int i;
    int frameIDX;
    //int j;
    clock_t time;
    time = clock();


    //check cache for the val
    for (i=0; i<cache_size; i++) {
	    if (cb[i].frameNum == frm && cb[i].slot == 1) {
		    cb[i].timeAccessed = time;
		    memcpy(cb[i].data, buf, BLOCK_FRAME_SIZE);
		    return 0;
	    }	    
    }
    frameIDX = 0;
    
    //find empty slot and implement LRU
    for(i=0; i<cache_size; i++) {
	if (cb[i].slot == 0) {
	    frameIDX = i;
	    break;
	}    
	else if (cb[i].timeAccessed<cb[frameIDX].timeAccessed) {
	    frameIDX = i;
	} 
    }
  
    	    
    memcpy(cb[frameIDX].data, buf, BLOCK_FRAME_SIZE);
    cb[frameIDX].frameNum = frm;
    cb[frameIDX].timeAccessed = time;    
    cb[frameIDX].slot = 1;

		
          	   
    return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : get_block_cache
// Description  : Get an frame from the cache (and return it)
//
// Inputs       : block - the block number of the block to find
//                frm - the  number of the frame to find
// Outputs      : pointer to cached frame or NULL if not found

void* get_block_cache(BlockIndex block, BlockFrameIndex frm)
{	
    int i;
    clock_t time;
    time = clock();
    //check cache and return if found
    for(i=0; i<cache_size; i++) {
	if (cb[i].frameNum == frm && cb[i].slot == 1) {
	    cb[i].timeAccessed = time;	
	    return cb[i].data;
	}
    }	


    return (NULL);

}


//
// Unit test

////////////////////////////////////////////////////////////////////////////////
//
// Function     : blockCacheUnitTest
// Description  : Run a UNIT test checking the cache implementation
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int blockCacheUnitTest(void)
{
    // Return successfully
    //logMessage(LOG_OUTPUT_LEVEL, "Cache unit test completed successfully.");
    return (0);
}
