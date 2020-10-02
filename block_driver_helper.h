#ifndef BLOCK_DRIVER_HELPER_H
#define BLOCK_DRIVER_HELPER_H

#include <stdint.h>

#include <block_controller.h>
#include <block_driver.h>

#define OPEN 1
#define CLOSED 0

typedef char frame_t[BLOCK_FRAME_SIZE];

struct file_data {
    char name[128];
    int size;
    uint16_t frames[1024];
    int nrFrames;
};
typedef struct file_data file_t;

struct file_handler {
    file_t* file;
    int loc;
    int status;
};
typedef struct file_handler fh_t;

extern int compute_frame_checksum(void* frame, uint32_t* cs1);

BlockXferRegister pack(uint32_t ky1, uint32_t fm1, uint32_t cs1, uint32_t rt1);
void unpack(BlockXferRegister reg, uint32_t* ky1, uint32_t* fm1, uint32_t* cs1, uint32_t* rt1);
void closeAllFiles(fh_t* handles);
int createNewFile(const char* path, file_t* file);
int openFile(fh_t* handle, file_t* file);
void closeFile(fh_t* handle);
int verify_cs1(frame_t frame, uint32_t cs1);
void executeOpcode(frame_t frame, uint32_t ky1, uint32_t fm1);
int allocateNewFrames(fh_t* handle, int32_t count);
int getNbFiles(file_t* files);
int getFreeFrame(file_t* files);

#endif // BLOCK_DRIVER_HELPER_H