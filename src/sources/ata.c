#include <stdint.h>
#include "../lib/ata.h"
#include "../lib/io.h"
void ata_read_sector(uint32_t lba, uint8_t seccnt, uint8_t* buf) {
    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F)); /* master + LBA bits 24..27 */
    outb(ATA_SECCNT, seccnt);
    outb(ATA_LBA0, (uint8_t)(lba));
    outb(ATA_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_LBA2, (uint8_t)(lba >> 16));
    outb(ATA_CMD, 0x20); /* READ SECTORS */

    /* Attendre BSY=0 et DRQ=1 */
    uint8_t st;
        do { st = inb(ATA_STATUS); } while (st & 0x80);
        while (!(st & 0x08)) { st = inb(ATA_STATUS); }
    for (int i = 0; i < 256 * seccnt; i++) {
        uint16_t w = inw(ATA_DATA);
        buf[i*2]   = (uint8_t)(w & 0xFF);
        buf[i*2+1] = (uint8_t)(w >> 8);
    }
    io_wait();
}
void ata_write_sector(uint32_t lba, uint8_t seccnt, uint8_t* buf) {
    // Sélection du disque master + LBA bits 24..27
    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));

    // Nombre de secteurs à écrire
    outb(ATA_SECCNT, seccnt);

    // Adresse LBA 0..23
    outb(ATA_LBA0, (uint8_t)(lba));
    outb(ATA_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_LBA2, (uint8_t)(lba >> 16));

    // Commande WRITE SECTORS
    outb(ATA_CMD, 0x30);

    // Attendre BSY=0 et DRQ=1
    uint8_t st;
    do { st = inb(ATA_STATUS); } while (st & 0x80);      // BSY=0
    while (!(st & 0x08)) { st = inb(ATA_STATUS); }       // DRQ=1

    // Écriture des 256 mots (512 octets)
    for (int i = 0; i < 256 * seccnt; i++) {
        uint16_t w = buf[i*2] | (buf[i*2+1] << 8);
        outw(ATA_DATA, w);
    }
    do { st = inb(ATA_STATUS); } while (st & 0x80); 
    io_wait(); // petite pause pour sécurité
}