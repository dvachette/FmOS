CC = ../opt/cross/bin/x86_64-elf-gcc
LD = ../opt/cross/bin/x86_64-elf-ld

CFLAGS = -m64 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -fno-pic
LDFLAGS = -T linker.ld -nostdlib

OBJDIR = objects
BINDIR = bin

OBJS = $(OBJDIR)/boot.o $(OBJDIR)/kernel.o $(OBJDIR)/io.o $(OBJDIR)/vga.o $(OBJDIR)/ata.o $(OBJDIR)/mnemosyne.o
KERNEL = $(BINDIR)/kernel.bin
ISO = $(BINDIR)/FmOS.iso
DISK = $(BINDIR)/disk.img

all: $(ISO) $(DISK)

# Compilation des objets
$(OBJDIR)/boot.o: boot.s | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/kernel.o: kernel.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/io.o: sources/io.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/vga.o: sources/vga.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/ata.o: sources/ata.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/mnemosyne.o: sources/mnemosyne.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
# Link du kernel
$(KERNEL): $(OBJS) | $(BINDIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Construction de l'ISO bootable avec GRUB
$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/kernel.bin
	echo 'set timeout=0' > isodir/boot/grub/grub.cfg
	echo 'set default=0' >> isodir/boot/grub/grub.cfg
	echo 'menuentry "FmOS" {' >> isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

# Création d'un disque virtuel de 10 Mo
$(DISK): | $(BINDIR)
	qemu-img create -f raw $@ 10M
	# écriture de données connues pour test (facultatif)
	# echo -n "HELLODISK" | dd of=$@ bs=1 conv=notrunc

# Lancer QEMU avec le CD et le disque
run: $(ISO) $(DISK)
	qemu-system-x86_64 -cdrom $(ISO) -drive file=$(DISK),format=raw,if=ide

# Création des répertoires si besoin
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/*.bin $(BINDIR)/*.iso $(BINDIR)/*.img
	rm -rf isodir
