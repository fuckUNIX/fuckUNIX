#include "display.h"
#include "ports.h"
#include <stdint.h>
#include "../kernel/mem.h"
#include "../kernel/util.h"

static char hex_chars[] = "0123456789ABCDEF";

// prints one hex byte
void print_hex_byte(uint8_t b) {
    char hex[2];
    hex[0] = hex_chars[(b >> 4) & 0x0F];
    hex[1] = hex_chars[b & 0x0F];
    print_string(hex);
}

// prints hex data of a certain lenght
void print_hex(uint8_t *data, int length) {
    for (int i = 0; i < length; i++) {
        print_hex_byte(data[i]);
        print_string(" ");
        if ((i + 1) % 16 == 0) print_nl();
    }
}

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

int move_offset_to_new_line(int offset) {
    return get_offset(0, get_row_from_offset(offset) + 1);
}

void set_char_at_video_memory(char character, int offset) {
    uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

int scroll_ln(int offset) {
    memory_copy(
            (uint8_t * )(get_offset(0, 1) + VIDEO_ADDRESS),
            (uint8_t * )(get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}

/*
 * TODO:
 * - handle illegal offset (print error message somewhere)
 */
void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_at_video_memory(string[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}

void print_nl() {
    int newOffset = move_offset_to_new_line(get_cursor());
    if (newOffset >= MAX_ROWS * MAX_COLS * 2) {
        newOffset = scroll_ln(newOffset);
    }
    set_cursor(newOffset);
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

void print_backspace() {
    int newCursor = get_cursor() - 2;
    set_char_at_video_memory(' ', newCursor);
    set_cursor(newCursor);
}

// DEPRICATED!!!
// Prints text in the middle of the screen in a message box.
void print_msgbox(char *text) {
    int startx = (MAX_COLS - BOX_WIDTH) / 2;
    int starty = (MAX_ROWS - BOX_HEIGHT) / 2;
    int offset;

    // Draw box
    for (int y = 0; y < BOX_HEIGHT; y++) {
        for (int x = 0; x < BOX_WIDTH; x++) {
            offset = 2 * ((starty + y) * MAX_COLS + (startx + x));

            if (y == 0) 
                set_char_at_video_memory(BOX_CHAR_BORDER, offset); // top border
            else if (y == BOX_HEIGHT - 1)
                set_char_at_video_memory('\\', offset);           // bottom border
            else if (x == 0 || x == BOX_WIDTH - 1)
                set_char_at_video_memory(BOX_CHAR_VERTICAL, offset); // vertical sides
            else
                set_char_at_video_memory(' ', offset);             // inside
        }
    }

    // Print text in center
    int msglen = 0;
    while (text[msglen]) msglen++;
    int msgx = startx + (BOX_WIDTH - msglen) / 2;
    int msgy = starty + BOX_HEIGHT / 2;
    offset = 2 * (msgy * MAX_COLS + msgx);
    for (int i = 0; i < msglen; i++) {
        set_char_at_video_memory(text[i], offset + i * 2);
    }

    delay(1000);

    // Optional: clear box after delay
    for (int y = 0; y < BOX_HEIGHT; y++) {
        for (int x = 0; x < BOX_WIDTH; x++) {
            offset = 2 * ((starty + y) * MAX_COLS + (startx + x));
            set_char_at_video_memory(' ', offset);
        }
    }
}