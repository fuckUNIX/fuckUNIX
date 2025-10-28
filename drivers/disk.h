// drivers/disk.h

#pragma once
#include <stdint.h>


void ata_lba_read(uint32_t lba, uint8_t count, uint8_t *buffer);
