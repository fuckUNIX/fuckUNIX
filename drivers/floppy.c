#include <stdint.h>
#include "../cpu/isr.h"

// your provided functions
unsigned char port_byte_in(uint16_t port);
void port_byte_out(uint16_t port, uint8_t data);
unsigned short port_word_in(uint16_t port);
void port_word_out(uint16_t port, uint16_t data);

// Standard floppy controller I/O ports
#define FDC_DOR   0x3F2
#define FDC_MSR   0x3F4
#define FDC_FIFO  0x3F5
#define FDC_CCR   0x3F7

// Floppy commands
#define FDC_CMD_READ  0x46
#define FDC_CMD_WRITE 0x45

#define SECTORS_PER_TRACK 18
#define BYTES_PER_SECTOR 512

static volatile uint8_t floppy_irq_fired = 0;

void floppy_irq(registers_t *r) {
    floppy_irq_fired = 1;
}

static void floppy_wait_irq() {
    while (!floppy_irq_fired);
    floppy_irq_fired = 0;
}

static void floppy_send(uint8_t byte) {
    // Wait until data register ready
    for (;;) {
        uint8_t msr = port_byte_in(FDC_MSR);
        if (msr & 0x80) break;
    }
    port_byte_out(FDC_FIFO, byte);
}

static uint8_t floppy_recv() {
    for (;;) {
        uint8_t msr = port_byte_in(FDC_MSR);
        if (msr & 0x80) break;
    }
    return port_byte_in(FDC_FIFO);
}

static void floppy_motor_on() {
    port_byte_out(FDC_DOR, 0x1C); // enable motor + DMA + controller
}

static void floppy_motor_off() {
    port_byte_out(FDC_DOR, 0x0C); // disable motor
}

void floppy_init() {
    // IRQ6 = interrupt 38
    register_interrupt_handler(38, floppy_irq);
    floppy_motor_on();
}

int floppy_read_sector(uint8_t head, uint8_t track, uint8_t sector, void *buffer) {
    floppy_motor_on();

    // TODO: setup DMA for read
    // dma_read_floppy((uint32_t)buffer, BYTES_PER_SECTOR);

    // send read command
    floppy_send(FDC_CMD_READ);
    floppy_send((head << 2) | 0); // drive = 0
    floppy_send(track);
    floppy_send(head);
    floppy_send(sector);
    floppy_send(2); // 512 bytes per sector
    floppy_send(SECTORS_PER_TRACK);
    floppy_send(0x1B);
    floppy_send(0xFF);

    floppy_wait_irq();

    uint8_t st0 = floppy_recv();
    uint8_t st1 = floppy_recv();
    uint8_t st2 = floppy_recv();
    (void)st0; (void)st1; (void)st2;

    floppy_motor_off();
    return 0;
}

int floppy_write_sector(uint8_t head, uint8_t track, uint8_t sector, const void *buffer) {
    floppy_motor_on();

    // TODO: setup DMA for write
    // dma_write_floppy((uint32_t)buffer, BYTES_PER_SECTOR);

    floppy_send(FDC_CMD_WRITE);
    floppy_send((head << 2) | 0);
    floppy_send(track);
    floppy_send(head);
    floppy_send(sector);
    floppy_send(2);
    floppy_send(SECTORS_PER_TRACK);
    floppy_send(0x1B);
    floppy_send(0xFF);

    floppy_wait_irq();

    uint8_t st0 = floppy_recv();
    uint8_t st1 = floppy_recv();
    uint8_t st2 = floppy_recv();
    (void)st0; (void)st1; (void)st2;

    floppy_motor_off();
    return 0;
}
