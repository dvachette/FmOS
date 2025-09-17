/* boot.s : bootloader minimal 64-bit compatible GRUB */

/* Multiboot header pour que GRUB charge notre noyau */
.set MAGIC,    0x1BADB002         /* Magic number Multiboot */
.set FLAGS,    0                  /* Flags (0 = pas d’options spécifiques) */
.set CHECKSUM, -(MAGIC + FLAGS)  /* Checksum pour valider le header */

/* --- Section Multiboot (obligatoire pour GRUB) --- */
.section .multiboot,"a"           /* Section allouée en lecture seule */
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* --- Entrée 32-bit (GRUB démarre en mode protégé 32-bit, paging désactivé) --- */
.section .text32,"ax"
.code32
.globl _start
_start:
    /* Initialisation de la pile 32-bit */
    movl $stack_top32, %esp       /* ESP pointe sur le haut de stack32 */

    /* Charger notre GDT (Global Descriptor Table) */
    lgdt gdt_desc                  /* Registre GDTR pointe sur gdt_desc */

    /* --- Création des tables de pages pour passage en long mode --- */
    /* PML4, PDPT, PD pour identité 0..4MiB, avec pages 2MiB */

    movl $pdpt, %eax               /* Adresse du PDPT */
    orl  $0x3, %eax                /* Flags P (présent) | RW (lecture/écriture) */
    movl %eax, pml4                /* Premier entry PML4 pointe sur PDPT */
    movl $0, pml4+4                /* Deuxième entry = 0 (réservé) */

    movl $pd, %eax
    orl  $0x3, %eax
    movl %eax, pdpt                 /* PDPT[0] pointe sur PD */
    movl $0, pdpt+4

    /* Configuration des PD (Page Directory) */
    movl $0x00000083, %eax          /* PD[0] = page 2MiB, P|RW|PS */
    movl %eax, pd
    movl $0, pd+4                   /* Entrée suivante vide */
    movl $0x00200083, %eax          /* PD[1] = page 2MiB, base=2MiB | P|RW|PS */
    movl %eax, pd+8
    movl $0, pd+12

    /* Charger CR3 avec PML4 */
    movl $pml4, %eax
    movl %eax, %cr3

    /* Activer PAE (Physical Address Extension) */
    movl %cr4, %eax
    orl  $0x20, %eax
    movl %eax, %cr4

    /* Activer LME dans EFER (MSR 0xC0000080) pour 64-bit */
    movl $0xC0000080, %ecx         /* Adresse MSR EFER */
    rdmsr                           /* Lire MSR dans EDX:EAX */
    orl  $0x00000100, %eax          /* Activer LME */
    wrmsr                           /* Écrire retour dans MSR */

    /* Activer le paging via CR0 */
    movl %cr0, %eax
    orl  $0x80000000, %eax          /* Set PG bit */
    movl %eax, %cr0

    /* --- Saut vers code 64-bit --- */
    ljmp $0x08, $long_mode_entry    /* CS=0x08 (code 64-bit) */

/* --- Section 64-bit --- */
.section .text,"ax"
.code64
long_mode_entry:
    /* Initialisation des segments de données 64-bit */
    movw $0x10, %ax                 /* Sélecteur data */
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es

    /* Initialisation de la pile 64-bit */
    leaq stack_top64(%rip), %rsp    /* RSP pointe vers stack64 */

    cld                              /* Direction flag = forward pour les string ops */

    /* Appel du noyau principal en C */
    .extern kernel_main
    call kernel_main

/* Boucle infinie pour bloquer le CPU après kernel_main */
.hang:
    hlt
    jmp .hang

/* --- GDT minimal pour long mode --- */
.align 16
gdt:
    .quad 0                        /* Segment null */

    /* Code segment 64-bit : accès 0x9A, flags L=1 (64-bit) */
    .word 0, 0
    .byte 0
    .byte 0x9A
    .byte 0x20
    .byte 0

    /* Data segment 64-bit : accès 0x92, flags L=0 */
    .word 0, 0
    .byte 0
    .byte 0x92
    .byte 0x00
    .byte 0

gdt_end:

gdt_desc:
    .word gdt_end - gdt - 1         /* Taille du GDT -1 */
    .long gdt                        /* Adresse de la GDT */

/* --- Tables de pages --- */
.bss
.align 4096
pml4:   .quad 0
.align 4096
pdpt:   .quad 0
.align 4096
pd:
    .quad 0
    .fill 511,8,0                   /* Remplissage de 511 qwords supplémentaires à 0 */

/* --- Piles --- */
.align 16
stack32:
    .space 4096                     /* Pile 32-bit (4 KiB) */
stack_top32:
.align 16
stack64:
    .space 8192                     /* Pile 64-bit (8 KiB) */
stack_top64:
