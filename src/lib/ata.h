#ifndef ATA_H_INCLUDED
#define ATA_H_INCLUDED
#include <stdint.h>
#define ATA_DATA     0x1F0
#define ATA_SECCNT   0x1F2
#define ATA_LBA0     0x1F3
#define ATA_LBA1     0x1F4
#define ATA_LBA2     0x1F5
#define ATA_DRIVE    0x1F6
#define ATA_CMD      0x1F7
#define ATA_STATUS   0x1F7

void ata_read_sector(uint32_t lba, uint8_t seccnt, uint8_t* buf);
void ata_write_sector(uint32_t lba, uint8_t seccnt, uint8_t* buf);

#endif // ATA_H_INCLUDED