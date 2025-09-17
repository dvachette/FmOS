// kernel.c
#include <stdint.h>
#include <stddef.h>
#include "lib/vga.h"
#include "lib/ata.h"
#include "lib/io.h"
static const char * __kernel_name__ = "Ouranos";
/* ---------- Entrée du noyau ---------- */
void kernel_main(void) {
    puts("FmOS x86_64 ready.\n");
    puts("Writing in disk \n");

    volatile uint8_t write[512];
    write[0] = 'H';
    write[1] = 'e';
    write[2] = 'l';
    write[3] = 'l';
    write[4] = 'o';

    for (int i = 5; i < 512; i++) {
        write[i] = 0x0F;
    }
    ata_write_sector(0, 1, write);

    // Reading a disk sector (512 bytes)
    puts("Reading LBA 0: ");
    uint8_t sector[512];
    ata_read_sector(0, 1, sector);
    puts("Done reading disk \n");
    for (int i = 0; i < 16; i++) {
        puthex8(sector[i]); vga_putc(' ');
    }

    puts("\nDone.\n");
}
