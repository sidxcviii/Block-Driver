#include <stdint.h>
#include <string.h>

// Project Includes
#include <block_controller.h>
#include <block_driver.h>
#include <block_driver_helper.h>
#include <cmpsc311_util.h>

extern int freeFrameNr;

// Packs the given register
BlockXferRegister pack(uint32_t ky1, uint32_t fm1, uint32_t cs1, uint32_t rt1)
{
    return ((uint64_t)ky1 << 56 | (uint64_t)fm1 << 40 | (uint64_t)cs1 << 8 | rt1);
}

// Unpacks the given register
void unpack(BlockXferRegister reg, uint32_t* ky1, uint32_t* fm1, uint32_t* cs1, uint32_t* rt1)
{
    *ky1 = reg >> 56;
    *fm1 = (reg << 8) >> 48;
    *cs1 = (reg << 24) >> 32;
    *rt1 = (reg << 56) >> 56;
    return;
}

// Closes all the file handles
void closeAllFiles(fh_t* handles)
{
    int i;
    for (i = 0; i < BLOCK_MAX_TOTAL_FILES; i++) {
        closeFile(&handles[i]);
    }
    return;
}

// Creates a new file with the given path
int createNewFile(const char* path, file_t* file)
{
    memcpy(file->name, path, strlen(path));
    file->size = 0;
    file->nrFrames = 0;
    return 0;
}

// Opens a new file handle to the given file
int openFile(fh_t* handle, file_t* file)
{
    handle->file = file;
    handle->loc = 0;
    handle->status = OPEN;
    return 0;
}

// Closes the given file handle
void closeFile(fh_t* handle)
{
    handle->status = CLOSED;
    handle->loc = -1;
    handle->file = NULL;
    return;
}

// Compares the checksum of the frame to the given checksum
int verify_cs1(frame_t frame, uint32_t cs1)
{
    uint32_t cs1_compute;
    compute_frame_checksum(frame, &cs1_compute);
    if (cs1 == cs1_compute) {
        return 0;
    } else {
        return -1;
    }
}

// Given a frame buffer, an instruction and a frame number,
// executes the instruction
void executeOpcode(frame_t frame, uint32_t ky1, uint32_t fm1)
{
    uint32_t rt1, cs1, cs1_comp;
    BlockXferRegister regstate;
    rt1 = -1;
    while (rt1 != 0) {
        if (ky1 == BLOCK_OP_WRFRME) {
            compute_frame_checksum(frame, &cs1);
        } else {
            cs1 = 0;
        }
        regstate = pack(ky1, fm1, cs1, 0);
        regstate = block_io_bus(regstate, frame);
        unpack(regstate, &ky1, &fm1, &cs1, &rt1);
        if (ky1 == BLOCK_OP_RDFRME) {
            compute_frame_checksum(frame, &cs1_comp);
            rt1 = (cs1 == cs1_comp) ? 0 : -1;
        }
    }
    return;
}

// Given a file handle and a number of bytes to write to a file,
// allocates as many frames as required to the file
int allocateNewFrames(fh_t* handle, int32_t count)
{
    uint16_t nrFrames;
    int32_t loc;
    nrFrames = handle->file->nrFrames;
    loc = handle->loc;
    while (loc + count > nrFrames * BLOCK_FRAME_SIZE) {
        handle->file->frames[nrFrames] = freeFrameNr;
        freeFrameNr++;
        nrFrames++;
        //  If we go over the max amount of frames, return -1
        if (freeFrameNr > BLOCK_BLOCK_SIZE) {
            return -1;
        }
    }
    handle->file->nrFrames = nrFrames;
    return 0;
}

// Given an array of files, returns the number of files
int getNbFiles(file_t* files)
{
    int i;
    for (i = 0; i < BLOCK_MAX_TOTAL_FILES; i++) {
        if (strlen(files[i].name) == 0) {
            return i;
        }
    }
    return i;
}

// Given an array of file_t, returns the number of the first frame unused by all the files
int getFreeFrame(file_t* files)
{
    int i;
    int j;
    int8_t frames[BLOCK_BLOCK_SIZE];
    memset(frames, 0, BLOCK_BLOCK_SIZE);
    // Set all the used frames to -1
    for (i = BLOCK_MAX_TOTAL_FILES; i < BLOCK_MAX_TOTAL_FILES; i++) {
        for (j = 0; j < files[i].nrFrames; j++) {
            frames[files[i].frames[j]] = -1;
        }
    }
    // Search for the first frame that is unused (i.e. has a 0 in the `frames` array)
    for (i = BLOCK_MAX_TOTAL_FILES; i < BLOCK_BLOCK_SIZE; i++) {
        if (frames[i] == 0) {
            return i;
        }
    }
    return -1;
}