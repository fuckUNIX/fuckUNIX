#include <stdint.h>
#include "../drivers/ports.h"

#define COM1 0x3F8

void serial_init() {
    port_byte_out(COM1 + 1, 0x00);    // Disable interrupts
    port_byte_out(COM1 + 3, 0x80);    // Enable DLAB
    port_byte_out(COM1 + 0, 0x03);    // Set divisor to 3 (38400 baud)
    port_byte_out(COM1 + 1, 0x00);    // High byte of divisor
    port_byte_out(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    port_byte_out(COM1 + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    port_byte_out(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_is_transmit_empty() {
    return port_byte_in(COM1 + 5) & 0x20;
}

void serial_write_char(char a) {
    while (serial_is_transmit_empty() == 0);
    port_byte_out(COM1, a);
}

void serial_write_string(const char* str) {
    while (*str) serial_write_char(*str++);
}
