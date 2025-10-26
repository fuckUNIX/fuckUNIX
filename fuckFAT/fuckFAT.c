#include "stddef.h"
#include "stdint.h"
#include "fuckFAT.h"

// include the drivers
#include "../drivers/display.h"

#define MAX_PATH 64
#define MAX_FILES 256

//typedef struct {
// char path[MAX_PATH];   // full path, e.g., "/binary/ls"
// uint32_t size;         // size of file in bytes
// uint8_t *data;         // pointer to file contents (or offset in image)
// } fuckfat_file_t;

void init_fuckFAT() {
    // print_string("Hello fuckFAT and fuckUNIX they suck and i hate them!!!!");
    // basic stub to be made when i fix the ATA driver.
};