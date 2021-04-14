#ifndef BLOCK_FRAME_CACHE_INCLUDED
#define BLOCK_FRAME_CACHE_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File           : block_cache.h
//  Description    : This is the header file for the implementation of the
//                   frame cache for the BLOCK memory system driver.
//
//

// Includes
#include <block_controller.h>

// Defines
#define DEFAULT_BLOCK_FRAME_CACHE_SIZE 1024 // Default size for cache

///
// Cache Interfaces

int set_block_cache_size(uint32_t max_frames);
// Set the size of the cache (must be called before init)

int init_block_cache(void);
// Initialize the cache

int close_block_cache(void);
// Clear all of the contents of the cache, cleanup

int put_block_cache(BlockIndex blk, BlockFrameIndex frm, void* frame);
// Put an object into the object cache, evicting other items as necessary

void* get_block_cache(BlockIndex blk, BlockFrameIndex frm);
// Get an object from the cache (and return it)

//
// Unit test

int blockCacheUnitTest(void);
// Run a UNIT test checking the cache implementation

#endif
