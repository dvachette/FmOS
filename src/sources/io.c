#include <stdint.h>
void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0,%1" :: "a"(val), "Nd"(port));
}
void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0,%1" :: "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1,%0" : "=a"(ret) : "Nd"(port));
    return ret;
}
uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1,%0" : "=a"(ret) : "Nd"(port));
    return ret;
}
void io_wait(void) {
    /* 400 ns delay: lire 0x3F6 quatre fois */
    inb(0x3F6); inb(0x3F6); inb(0x3F6); inb(0x3F6);
}