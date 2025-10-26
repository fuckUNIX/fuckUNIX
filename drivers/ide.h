#pragma once
#include <stdint.h>

void ata_read_sector(uint8_t drive, uint8_t *buffer);
