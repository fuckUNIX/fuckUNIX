// drivers/serial.h

// provides basic serial functions for debugging

void serial_write_char(char a);
void serial_write_string(const char* str);
void serial_init();