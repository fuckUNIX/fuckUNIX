// drivers/disk.h

#pragma once
#include <stdint.h>

#define DISK_EMPTY_BUFFER [512]

int ata_read_sector(uint32_t lba, uint8_t* buffer);
void ata_write_sector(uint32_t lba, uint8_t *buffer);
void ata_read_sectors(uint32_t lba, uint32_t count, void *buffer);