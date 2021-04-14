#ifndef BLOCK_DRIVER_INCLUDED
#define BLOCK_DRIVER_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File           : block_driver.h
//  Description    : This is the header file for the standardized IO functions
//                   for used to access the BLOCK storage system.
//

//

// Include files
#include <stdint.h>

#include <block_driver_helper.h>

// Defines
#define BLOCK_MAX_TOTAL_FILES 1024 // Maximum number of files ever
#define BLOCK_MAX_PATH_LENGTH 128 // Maximum length of filename length
#define BLOCK_MAX_FRAME_PER_FILE 1024 // Maximum number of frames per file

//
// Interface functions

int32_t block_poweron(void);
// Startup up the BLOCK interface, initialize filesystem

int32_t block_poweroff(void);
// Shut down the BLOCK interface, close all files

int16_t block_open(char* path);
// This function opens the file and returns a file handle

int16_t block_close(int16_t fd);
// This function closes the file

int32_t block_read(int16_t fd, void* buf, int32_t count);
// Reads "count" bytes from the file handle "fh" into the buffer  "buf"

int32_t block_write(int16_t fd, void* buf, int32_t count);
// Writes "count" bytes to the file handle "fh" from the buffer  "buf"

int32_t block_seek(int16_t fd, uint32_t loc);
// Seek to specific point in the file

#endif
