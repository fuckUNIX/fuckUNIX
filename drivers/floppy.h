// drivers/floppy.h

#include <stdint.h>

void floppy_init(); // init function
int floppy_read_sector(uint8_t head, uint8_t track, uint8_t sector, void *buffer);
int floppy_write_sector(uint8_t head, uint8_t track, uint8_t sector, const void *buffer);