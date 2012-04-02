OBJDIR := obj
V    =  # V = for verbosity and V = @ for none
TOP  = $(shell pwd)
KERNEL_NAME  = "Tantra"

CC      := gcc -pipe
AS      := as
AR      := ar
LD      := ld
NM      := nm
OBJCOPY := objcopy
OBJDUMP := objdump
PERL    := perl
PYTHON  := python

QEMU    := qemu

# try to generate a unique GDB port
GDBPORT	:= $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -nographic -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

REMOVE_VM = $(shell if VBoxManage list vms | grep $(KERNEL_NAME); \
        then VBoxManage unregistervm $(KERNEL_NAME) --delete; fi)

# Compiler flags
# Only optimize to -O1 to discourage inlining, which complicates backtraces.
CFLAGS := $(CFLAGS) -O1 -fno-builtin -I$(TOP) -I$(TOP)/include/ -I$(TOP)/include/libc -MD
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -Wall -Wno-format -Wno-unused -Werror -gstabs -m32

# Add -fno-stack-protector if the option exists.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

# Kernel CFLAGS
KERNEL_CFLAGS := $(CFLAGS) -gstabs

# Common linker flags
LDFLAGS := -m elf_i386 -nostdlib

# Lists that the */Makefrag makefile fragments will add to
OBJDIRS :=

# Make sure that 'all' is the first target
all:

# Eliminate default suffix rules
.SUFFIXES:

# Delete target files if there is an error (or make is interrupted)
.DELETE_ON_ERROR:

# make it so that no intermediate .o files are ever deleted
.PRECIOUS: %.o $(OBJDIR)/boot/%.o $(OBJDIR)/kernel/%.o \
	   $(OBJDIR)/lib/%.o

# Include Makefrags for subdirectories
include boot/Makefrag
include kernel/Makefrag

IMAGES = $(OBJDIR)/kernel/kernel.img
QEMUOPTS = -hda $(OBJDIR)/kernel/kernel.img -serial mon:stdio

.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@

qemu: $(IMAGES)
	$(QEMU) $(QEMUOPTS)

qemu-gdb: $(IMAGES) .gdbinit
	@echo "***"
	@echo "*** Now run 'gdb'." 1>&2
	@echo "***"
	$(QEMU) $(QEMUOPTS) -S $(QEMUGDB)

# For deleting the build
clean:
	@echo "Cleaning build tree" $(REMOVE_VM)
	rm -rf $(OBJDIR) $(TOP)/staging/tantra* .gdbinit *.iso *.vdi

# This magic automatically generates makefile dependencies
# for header files included from C source files we compile,
# and keeps those dependencies up-to-date every time we recompile.
# See 'mergedep.pl' for more information.
$(OBJDIR)/.deps: $(foreach dir, $(OBJDIRS), $(wildcard $(OBJDIR)/$(dir)/*.d))
	@mkdir -p $(@D)
	@$(PERL) mergedep.pl $@ $^

-include $(OBJDIR)/.deps

always:
	@:

.PHONY: all always clean
