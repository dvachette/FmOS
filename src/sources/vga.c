#include <stdint.h>
#include <stddef.h>
#include "../lib/vga.h"

size_t cur_x = 0, cur_y = 0;
uint8_t attr = 0x07; /* gris sur noir */

void vga_putc(char c) {
    if (c == '\n') {
        cur_x = 0;
        if (++cur_y >= VGA_ROWS) cur_y = 0;
        return;
    }
    VGA_MEM[cur_y*VGA_COLS + cur_x] = ((uint16_t)attr << 8) | (uint8_t)c;
    if (++cur_x >= VGA_COLS) { cur_x = 0; if (++cur_y >= VGA_ROWS) cur_y = 0; }
}
void puts(const char* s) { while (*s) vga_putc(*s++); }
void puthex8(uint8_t b) {
    const char* d = "0123456789ABCDEF";
    char buf[3]; buf[0]=d[(b>>4)&0xF]; buf[1]=d[b&0xF]; buf[2]=0;
    puts(buf);
}