#ifndef FUCKFAT_H
#define FUCKFAT_H

#include <stdint.h>
#include <stddef.h>

// Max path length for a file
#define MAX_PATH 64

// Max number of files in the filesystem
#define MAX_FILES 256

// File structure
typedef struct {
    char path[MAX_PATH];   // full path, e.g., "/binary/ls"
    uint32_t size;         // size of file in bytes
    uint8_t *data;         // pointer to file contents (or offset in image)
} fuckfat_file_t;

// Filesystem table and counter
extern fuckfat_file_t fs_table[MAX_FILES];
extern int fs_count;

// Initialize FuckFAT
void init_fuckFAT(void);

#endif // FUCKFAT_H