KERNEL=kernel.elf
SRC=src/
OBJDIR=build/
ISO=iso/

.PHONY: $(KERNEL)
.PHONY: clean

AS=nasm
CC=gcc
LD=ld
INC_PATH=-I$(SRC)/include -I$(SRC)/include/libc -I$(SRC)
ASFLAGS=-f elf32
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector  -nostartfiles -nodefaultlibs -c -Wall -Wextra $(INC_PATH) -std=c11 -pedantic # -Werror
LDFLAGS=-T $(SRC)/link.ld -melf_i386

DOTFILES = boot/boot.o kmain.o io.o fb.o \
		  idt.o gdt.o descriptor_table.o \
		  interrupt.o isr.o \
		  timer.o keyboard.o paging.o

OBJ = $(patsubst %,$(OBJDIR)%,$(DOTFILES))

$(OBJDIR)%.o: $(SRC)%.c
	@echo [CC] $<
	@mkdir -p $(OBJDIR) $(OBJDIR)/boot
	@$(CC) $(CFLAGS)  -o $@ $<

$(OBJDIR)%.o: $(SRC)%.s
	@echo [AS] $<
	@mkdir -p $(OBJDIR) $(OBJDIR)/boot
	@$(AS) $(ASFLAGS) -o $@ $<

$(KERNEL): $(OBJ)
	@echo [INFO] Linking Kernel ELF
	@mkdir -p $(OBJDIR) $(OBJDIR)/boot
	@$(LD) $(LDFLAGS) $(OBJ) -o $(OBJDIR)/kernel.elf

all: $(KERNEL)

iso: $(KERNEL)
	@cp $(OBJDIR)/kernel.elf $(ISO)/kernel.elf
	@echo [INFO] Building ISO image
	@genisoimage -R                             \
				-b boot/grub/stage2_eltorito    \
				-no-emul-boot                   \
				-boot-load-size 4               \
				-A os                           \
				-input-charset utf8             \
				-quiet                          \
				-boot-info-table                \
				-o $(OBJDIR)tantra.iso          \
				iso

run: iso
	@echo [INFO] Starting kernel on Bochs
	@bochs -f utils/bochsrc.txt -q

qemu: clean iso
	@echo [INFO] Starting kernel on Qemu
	@qemu-system-i386 -m 32M -cdrom $(OBJDIR)tantra.iso

clean:
	@echo [INFO] Cleaning build directory and other artifacts
	@rm -rf $(OBJDIR) iso/kernel.elf tantra.iso
