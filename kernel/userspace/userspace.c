// fuckUNIX main userspace C source file

// Handles switching and more to userspace handles syscalls and a ton more in the future(hopefully!!)

#include <stdint.h>
#include "../mem.h"

#define USER_HEAP_START 0x01000000
#define USER_HEAP_SIZE  0x01000000

uint8_t* user_heap_top = (uint8_t*)USER_HEAP_START;

#define USER_STACK_SIZE 0x4000  // 16 KB

#define USER_CS 0x1B  // index 3 << 3 | RPL=3
#define USER_DS 0x23  // index 4 << 3 | RPL=3


// Put this somewhere in RAM, aligned
uint8_t user_program[] = {
    0xEB, 0xFE  // infinite loop: JMP $ (assembly: EB FE)
};


uint32_t allocate_user_stack() {
    // alloc() returns a pointer to the memory
    uint8_t* stack = (uint8_t*)alloc(USER_STACK_SIZE);
    if (!stack) return 0; // allocation failed
    // Return top of stack (stack grows downward)
    return (uint32_t)stack + USER_STACK_SIZE;
}

// user_code: physical address of user program
// user_stack: top of allocated user stack (stack grows downward)
void enter_user(uint32_t user_code, uint32_t user_stack) {
    asm volatile(".intel_syntax noprefix; ...");
    asm volatile(
    "cli;"
    "mov %[ds], %%ax;"
    "mov %%ax, %%ds;"
    "mov %%ax, %%es;"
    "mov %%ax, %%fs;"
    "mov %%ax, %%gs;"
    "push %[ds];"       // SS
    "push %[stack];"    // ESP
    "pushf;"
    "push %[cs];"       // CS
    "push %[code];"     // EIP
    "iret;"
    :
    : [stack] "r"(user_stack), [code] "r"(user_code),
    [cs] "r"(USER_CS), [ds] "r"(USER_DS)
    );

}


void init_userspace() {
    uint32_t user_stack = allocate_user_stack();
    enter_user((uint32_t)user_program, user_stack);
}
