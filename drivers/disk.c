#include <stdint.h>
#include "ports.h"

#define ATA_PRIMARY       0x80
#define ATA_DATA          (ATA_PRIMARY + 0)
#define ATA_ERROR         (ATA_PRIMARY + 1)
#define ATA_SECCOUNT0     (ATA_PRIMARY + 2)
#define ATA_LBA0          (ATA_PRIMARY + 3)
#define ATA_LBA1          (ATA_PRIMARY + 4)
#define ATA_LBA2          (ATA_PRIMARY + 5)
#define ATA_DRIVE         (ATA_PRIMARY + 6)
#define ATA_STATUS        (ATA_PRIMARY + 7)
#define ATA_COMMAND       (ATA_PRIMARY + 7)

#define ATA_CMD_READ      0x20
#define ATA_CMD_WRITE     0x30
#define ATA_CMD_FLUSH     0xE7
#define ATA_DRIVE_MASTER  0xE0

// Wait until BSY=0
static inline void ata_wait_busy(void) {
    while (port_byte_in(ATA_STATUS) & 0x80);
}

// Wait until DRQ=1 (ready for data transfer)
static inline void ata_wait_drq(void) {
    while (!(port_byte_in(ATA_STATUS) & 0x08));
}


// Read one 512-byte sector (LBA 0–28-bit range)
void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    ata_wait_busy();

    port_byte_out(ATA_DRIVE, ATA_DRIVE_MASTER | ((lba >> 24) & 0x0F));
    port_byte_out(ATA_SECCOUNT0, 1);
    port_byte_out(ATA_LBA0, (uint8_t)(lba));
    port_byte_out(ATA_LBA1, (uint8_t)(lba >> 8));
    port_byte_out(ATA_LBA2, (uint8_t)(lba >> 16));
    port_byte_out(ATA_COMMAND, ATA_CMD_READ);

    ata_wait_busy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        uint16_t data = port_word_in(ATA_DATA);
        buffer[i * 2]     = data & 0xFF;
        buffer[i * 2 + 1] = data >> 8;
    }
}

// Write one 512-byte sector (LBA 0–28-bit range)
void ata_write_sector(uint32_t lba, uint8_t *buffer) {
    ata_wait_busy();

    port_byte_out(ATA_DRIVE, ATA_DRIVE_MASTER | ((lba >> 24) & 0x0F));
    port_byte_out(ATA_SECCOUNT0, 1);
    port_byte_out(ATA_LBA0, (uint8_t)(lba));
    port_byte_out(ATA_LBA1, (uint8_t)(lba >> 8));
    port_byte_out(ATA_LBA2, (uint8_t)(lba >> 16));
    port_byte_out(ATA_COMMAND, ATA_CMD_WRITE);

    ata_wait_busy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        uint16_t data = (uint16_t)buffer[i * 2] | ((uint16_t)buffer[i * 2 + 1] << 8);
        port_word_out(ATA_DATA, data);
    }

    // Flush write cache (important for real drives)
    port_byte_out(ATA_COMMAND, ATA_CMD_FLUSH);
    ata_wait_busy();
}

void ata_read_sectors(uint32_t lba, uint32_t count, void *buffer) {
    for (uint32_t i = 0; i < count; i++)
        ata_read_sector(lba + i, (uint8_t*)buffer + i * 512);
}
