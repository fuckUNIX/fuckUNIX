#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "../fuckFAT/fuckFAT.h"

#include "../drivers/ide.h"

#include "util.h"
#include "mem.h"


#define boot_banner "--------------------------\n Welcome to fuckUNIX 1.0 \n--------------------------"
#define PANIC_BUF_SIZE 128

void* alloc(int n) {
    int *ptr = (int *) mem_alloc(n * sizeof(int));
    if (ptr == NULL_POINTER) {
        print_string("Memory not allocated.\n");
    } else {
        // Get the elements of the array
        for (int i = 0; i < n; ++i) {
//            ptr[i] = i + 1; // shorthand for *(ptr + i)
        }

        for (int i = 0; i < n; ++i) {
//            char str[256];
//            int_to_string(ptr[i], str);
//            print_string(str);
        }
//        print_nl();
    }
    return ptr;
}

void test_ATA() {
    uint8_t sector[512]; // define a sector aka 512 bytes of data per each sector
    ata_read_sector(0, sector); // read sector 0

    print_string("Sector 0 read:\n"); // let the user know it read the sector
    //for (int i = 0; i < 512; i++) {
    //    print_hex(sector[i]);
    //    if (i % 16 == 15) print_string("\n");
    // }
    print_hex(sector, 512); // replaced the above with one line, prints out the sector's hex
}

// prints the boot banner stored in the boot_banner variable
void print_banner() { 
    print_string(boot_banner);
}

void start_kernel() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();

    print_string("Initializing dynamic memory.\n");
    init_dynamic_mem();

    clear_screen();

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

    print_banner();
    print_string("\n>");
}


// kernel api start
void khalt() {
    asm volatile("hlt");
}

void kpoweroff() {
    print_string("Powering off the PC\n"); // TODO Implement ACPI
    khalt();
}

void kpanic(char *reason) {
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
}

void execute_command(char *input) {
    if (compare_string(input, "EXIT") == 0) {
        kpoweroff();
    }
    else if (compare_string(input, "") == 0) {
        print_string("\n> ");
    }
    else if (compare_string(input, "PANIC") == 0) {
        kpanic("User initiated panic!");
    }
    else if (compare_string(input, "ATATEST") == 0) {
        test_ATA();
        print_string("\n>");
    }
    else {
        print_string("Unknown command: ");
        print_string(input);
        print_string("\n> ");
    }
}
