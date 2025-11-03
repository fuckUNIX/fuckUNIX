#pragma once

#include <stdint.h>
#include <stddef.h>

#define NULL_POINTER ((void*)0)

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes);

int memory_compare(uint8_t *a, uint8_t *b, uint32_t len);

void init_dynamic_mem();

void print_dynamic_node_size();

void print_dynamic_mem();

void *mem_alloc(size_t size);

void mem_free(void *p);

void* alloc(int n);