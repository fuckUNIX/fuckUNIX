// drivers/disk.c

#include "disk.h"
#include "ports.h"
#include <stdint.h>

// Reads sectors from primary ATA drive using LBA
// lba: Logical Block Address
// count: number of sectors (1-255)
// buffer: pointer to memory to store sector data
void ata_lba_read(uint32_t lba, uint8_t count, uint8_t *buffer) {
    // Only 28-bit LBA supported here
    lba &= 0x0FFFFFFF;

    // Drive/head register: 0xE0 for master, plus bits 24-27 of LBA
    port_byte_out(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));

    // Sector count
    port_byte_out(0x1F2, count);

    // LBA low/mid/high bytes
    port_byte_out(0x1F3, (uint8_t)(lba & 0xFF));
    port_byte_out(0x1F4, (uint8_t)((lba >> 8) & 0xFF));
    port_byte_out(0x1F5, (uint8_t)((lba >> 16) & 0xFF));

    // Command: read sectors with retry
    port_byte_out(0x1F7, 0x20);

    // Wait until the drive sets DRQ (bit 3)
    while (!(port_byte_in(0x1F7) & 0x08));

    // Read data: 256 words per sector
    for (int s = 0; s < count; s++) {
        for (int i = 0; i < 256; i++) {
            uint16_t word = port_word_in(0x1F0);
            buffer[i*2]   = word & 0xFF;
            buffer[i*2+1] = (word >> 8) & 0xFF;
        }
        buffer += 512; // move buffer pointer to next sector
    }
}
