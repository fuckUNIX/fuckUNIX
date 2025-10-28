// kernel/disktest.c

#include <stdint.h>

#include "disktest.h"

#include "../drivers/display.h"
#include "../drivers/disk.h"

static uint8_t buffer[512]; // buffer for 1 sector
uint32_t sector = 0; // LBA of the sector to read
uint8_t count = 1;   // number of sectors



// reads a sector and prints its contents
void test_disk() {
    for (int i = 0; i < 16; i++) {
    print_hex_byte(buffer[i]); // only print, don’t execute
    print_string(" ");
}
print_nl();
}
