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

#include "fuckUNIX.h"

#include "util.h"
#include "mem.h"


#define boot_banner "--------------------------\n Welcome to fuckUNIX 1.0 \n--------------------------"
#define PANIC_BUF_SIZE 128

// prints the boot banner stored in the boot_banner variable
void print_banner() { 
    print_string(boot_banner);
}

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
    clear_screen(); // clears the screen

    load_drivers(); // loads all the drivers

    alloc_memory(); // allocate memory

    clear_screen(); // clears again

    print_banner(); // prints banner
    print_string("\n>");
}


// kernel api start
//void khalt() {
//    asm volatile("hlt");
//}

//void kpoweroff() {
//    print_string("Powering off the PC\n"); // TODO Implement ACPI
//    khalt();
//}

/*void kpanic(char *reason) {
    char buffer[PANIC_BUF_SIZE];
    int i = 0;

    // Copy fixed string
    char *fixed = "KERNEL PANIC!!!!! REASON: ";
    while (fixed[i] != '\0') {
        buffer[i] = fixed[i];
        i++;
    }

    // Copy reason string
    int j = 0;
    while (reason[j] != '\0' && i < PANIC_BUF_SIZE - 1) {
        buffer[i] = reason[j];
        i++;
        j++;
    }

    buffer[i] = '\0'; // null-terminate

    clear_screen();
    print_boxed(buffer);
    khalt();
}*/

// do not use right now as in the future it will be removed!!
void execute_command(char *input) {
    if (compare_string(input, "EXIT") == 0) {
        // stub!!
        print_string("Not implemented!!!");
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