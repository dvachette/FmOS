#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED
#include <stdint.h>

/*
FS Structure : units are in bytes
    Superblock :
        magic number (check if an fs is mounted) uint64 -> luhn valid (3158601305485541 for instance)
        Inode section size (in LBA) -> 4096 -> uint16
        Data section size -> 4096 -> uint16
        Data block size -> 8 -> uint8
        allocated -> uint32
        blocks bitmap : 00 -> free, 10 -> inode block, 01 -> data block, -> 11 not used yet (reserved) -> uint8[2048]

    Inode section :
        Inodes bitmap for this section -> uint8[512]

        Blocks bitmap for the corresponding data section -> uint8[512] 

        [Inodes][4094] -> 128 Bytes
            mode (file, dir, symlink, char_periph, bloc_periph, pipe, socket) [7 bits], rwxrwxrwx[9 bits] -> uint16
            gid ->uint16
            uid -> uint16
            filesize in bytes -> uint64
            alocated_size in blocks-> uint64
            ctime -> uint64
            mtime -> uint64
            atime -> uint64
            links count -> uint16
            data blocks LBA -> uint32[12]
            indirect1 -> uint32
            indirect2 -> uint32
            indirect3 -> uint32
            rdev -> uint16
            padding -> uint8[20]
        



    Data section :
        [DATA: {
            ...
        }]
        [DIRENT: {
            name : char[252]
            inode number : uint32

        }]
        [INDIRECT[1, 2, 3]:{
            data LBA : uint32 [1024]
        }]        

 */

typedef struct superblock_t {
    uint16_t magic;
    uint16_t inode_section_size; /* Size in bytes */
    uint16_t data_section_size; /* Size in bytes */
    uint8_t data_block_size; /* Size of a data block in Logic blocks*/
    uint32_t allocated_sections; /* Allocated sections */         
    uint8_t sections_bitmap[8192]; /* Bitmap of the disk's sections (00 -> free, 10 -> inode block, 01 -> data block, -> 11 not used yet (reserved))*/
} superblock_t; 


typedef struct inode_t {
    uint16_t mode; /* Mode (file, dir, symlink, char_periph, bloc_periph, pipe, socket) [7 bits], rwxrwxrwx[9 bits] */
    uint16_t gid;
    uint16_t uid;
    uint64_t filesize;
    uint64_t allocated; /* alocated_size in memory blocks of 8 data blocks */
    uint64_t ctime; /* Creation timestamp */
    uint64_t mtime; /* Last modified timestamp*/
    uint64_t atime; /* Last acces timestamp */
    uint16_t links_count; /* Number of hard links to this inode, if 0, it can be deleted or overwrited */
    uint32_t data_blocks[12]; /* LBA's of the direct data blocks */
    uint32_t indirect1;
    uint32_t indirect2;
    uint32_t indirect3;
    uint32_t rdev;
    uint8_t pading[18];
        
};
#endif // FS_H_INCLUDED