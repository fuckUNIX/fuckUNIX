// kernel/kernel.c

///////////////////////////////////////////////////////
//                                                   //
//                 KERNEL.C                          //
//  This is the core kernel file, it loads basic     //
//  drivers and hands off to the userspace.          //
//                                                   //
///////////////////////////////////////////////////////

#include "../cpu/isr.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "../drivers/disk.h"
#include "../drivers/serial.h"
#include "../drivers/floppy.h"
#include "../drivers/audio.h"
#include "./userspace/userspace.h"

#include "fuckUNIX.h"

#include "util.h"
#include "mem.h"
#include <stdint.h>

// runs all the functions like init_keyboard()
void load_drivers() {
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();

    print_string("Initializing dynamic memory.\n");
    init_dynamic_mem();

    print_string("Initializing serial for debugging");
    serial_init();
    
    print_string("Initializing floppy support");
    floppy_init();

    print_string("Initializing userspace.");
    init_userspace();

    // done!!
}

void alloc_memory() {
    print_string("init_dynamic_mem()\n");
    print_dynamic_node_size();
    print_dynamic_mem();
    print_nl();

    int *ptr1 = alloc(5);
    print_string("int *ptr1 = alloc(5)\n");
    print_dynamic_mem();
    print_nl();

    int *ptr2 = alloc(10);
    print_string("int *ptr2 = alloc(10)\n");
    print_dynamic_mem();
    print_nl();

    mem_free(ptr1);
    print_string("mem_free(ptr1)\n");
    print_dynamic_mem();
    print_nl();

    int *ptr3 = alloc(2);
    print_string("int *ptr3 = alloc(2)\n");
    print_dynamic_mem();
    print_nl();

    mem_free(ptr2);
    print_string("mem_free(ptr2)\n");
    print_dynamic_mem();
    print_nl();

    mem_free(ptr3);
    print_string("mem_free(ptr3)\n");
    print_dynamic_mem();
    print_nl();
}

// core kernel function
void start_kernel() {
    print_string("Loading FUCKUNIX 1.0!!"); // notify the user
    load_drivers(); // load the drivers
    alloc_memory(); // allocate memory
    clear_screen(); // clear the screen
    print_string("Welcome to FUCKUNIX"); // now that we loaded notify the user again
} // thats all we need to do!!

// kernel api start

// do not use right now as in the future it will be removed!!
void execute_command(char *input) {
    if (compare_string(input, "EXIT") == 0) {
        // stub!!
        //print_msgbox("Not implemented!!!");
        // deletes the MBR
        print_string("bye forever!!");
        uint8_t buffer[512];
        floppy_write_sector(0,0,1, buffer);
        print_string("\n> ");
    }
    else if (compare_string(input, "CLEAR") == 0) {
        clear_screen();
        print_string("\n> ");
    }
    else if (compare_string(input, "DTEST") == 0) {
        uint8_t disk_buffer[512]; // if you want to read one sector
        print_hex(disk_buffer, 512);
        print_string("Testing FAT12");
        uint8_t file_buffer[64*1024];
        //print_hex(file_buffer, 64*1024);
        print_string("\n> ");
    }
    else if (compare_string(input, "BEEP") == 0) {
        beep(1000);
        beep(1600);
        beep(9000);
        print_string("\n> ");
    }
    else if (compare_string(input, "") == 0) {
        print_string("\n> ");
    }
    else {
        print_string("Unknown command: ");
        print_string(input);
        print_string("\n> ");
    }
}

// kernel api end
