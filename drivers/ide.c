#include "ide.h"
#include "ports.h"
#include "../drivers/display.h"
#include <stdint.h>

#define ATA_PRIMARY_CMD  0x1F0
#define ATA_PRIMARY_CTRL 0x3F6
#define ATA_STATUS      0x1F7
#define ATA_DATA        0x1F0

// Status flags
#define ATA_BSY 0x80
#define ATA_DRQ 0x08

static inline uint8_t ata_status() {
    uint8_t status;
    __asm__ volatile("inb %1, %0" : "=a"(status) : "Nd"(ATA_STATUS));
    return status;
}

static void ata_wait_not_busy() {
    while (ata_status() & ATA_BSY);
}

static void ata_wait_drq() {
    while (!(ata_status() & ATA_DRQ));
}

// Reads 512 bytes from the given drive (0 = master)
void ata_read_sector(uint8_t drive, uint8_t *buffer) {
    ata_wait_not_busy();

    // Select drive 0 (master) and LBA=0
    port_byte_out(0x1F6, 0xE0 | (drive << 4));
    port_byte_out(0x1F2, 1); // sectors to read
    port_byte_out(0x1F3, 0); // LBA low
    port_byte_out(0x1F4, 0); // LBA mid
    port_byte_out(0x1F5, 0); // LBA high
    port_byte_out(0x1F7, 0x20); // READ SECTOR command


    ata_wait_drq();

    // Read 256 words = 512 bytes
    for (int i = 0; i < 256; i++) {
        uint16_t val = port_word_in(0x1F0);
        buffer[i*2] = val & 0xFF;
        buffer[i*2+1] = (val >> 8) & 0xFF;
    }
}
