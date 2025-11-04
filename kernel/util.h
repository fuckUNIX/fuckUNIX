#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

int string_length(char s[]);

void reverse(char s[]);

void int_to_string(int n, char str[]);

bool backspace(char s[]);

void append(char s[], char n);

int compare_string(char s1[], char s2[]);

void delay(int ticks);

int strlen(char *str);

int strlen(char *str);

char* strncpy(char* dest, const char* src, size_t n) ;