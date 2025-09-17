#ifndef VGA_H_INCLUDED
#define VGA_H_INCLUDED
#include <stdint.h>

#define VGA_MEM ((volatile uint16_t*)0xB8000)
#define VGA_COLS 80
#define VGA_ROWS 25


void vga_putc(char c);
void puts(const char* s);
void puthex8(uint8_t b);


#endif // VGA_H_INCLUDED
