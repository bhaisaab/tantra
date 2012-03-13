#include <mmu.h>

# This code runs from the first sector of booting media into memory at
# physical address 0x7c00 and starts executing in real mode with
# %cs=0 %ip=7c00

.code16
.globl start                           # Address is 0x7c00
start:
  cli                                  # Disable interrupts
  xorw    %ax, %ax                     # Set segments to zero
  movw    %ax, %ds                     # to insure it's .org 0x7c00
  movw    %ax, %es
  movw    %ax, %ss

# Enable A20: Fix IBM's mistake
# The A20 gate is a bit in the keyboard's 8042 controller that enables or
# disables 21st address line. To change the A20 gate, we access hardware
# port 0x64 and 0x60 of 8042 chip:
#   0x60 read  -> output register to get data from keyboard
#   0x60 write -> data register for sending kbd controller commands
#   0x64 read  -> status register that can be read at anytime for kbd status
#   0x64 write -> commmand register to set controller options, the A20 gate

enable_A20:
iowait_8042.1:
  inb     $0x64, %al
  testb   $0x2, %al
  jnz     iowait_8042.1

  movb    $0xd1, %al                    # write 0xd1 -> port 0x64
  outb    %al, $0x64

iowait_8042.2:
  inb     $0x64, %al
  testb   $0x2, %al
  jnz     iowait_8042.2

  movb    $0xdf, %al                    # write 0xdf -> port 0x60, A20 Enabled!
  outb    %al, $0x60

load_gdt:
  lgdt    gdt_table                    # Bootstraps GDT
  movl    %cr0, %eax                   # Switch to protected mode
  orl     $CR0_PE, %eax                # Enable gate for protected mode
  movl    %eax, %cr0                   # Enter Protected Mode

# Now complete transition to 32-bit protected mode by indirectly modifying the
# %cs using ljmp to reload %cs : %eip, use GDT[1] i.e. the kernel code segment
# As each entry in the gdt table is 8 bytes long, hence we use a <<3 to select
# that particular SEG_ entry
  ljmp    $(SEG_KCODE<<3), $pmode      # Far jump, but in 32-bit code segment

.code32                                # Assemble for 32-bit mode
pmode:
  movw    $(SEG_KDATA<<3), %ax         # Get our kernel data segment selector
  movw    %ax, %ds                     # Initialize data, extra and stack segment
  movw    %ax, %es
  movw    %ax, %ss
  movw    %ax, %fs
  movw    %ax, %gs

  # Set up the stack pointer and call the C function
  # stack grows down from start:0x7c00 to 0x0000, as memory from
  # 0xa0000 to 0x100000 is littered with device memory regions
  movl    $start, %esp
  call    masterboot

spin:
  jmp     spin

# Bootstrap GDT, each entry is 8 bytes long
# We'll use paging hardware instead, so have base address equal to 0x0
# and maximum limit equal to 4GB-1 (max usable memory on an x86 system)
# and use SEG_ASM(type,base,lim) appropriately for code and data
# This way in protected mode, logical addresses map 1:1 to physical ones

.p2align 2                             # force 4 byte alignment
gdt:
  SEG_NULLASM                          # GDT[0], Null segment: dummy
  SEG_ASM(STA_R|STA_X, 0x0,0xffffffff) # GDT[1], Code segment: read, executable only
  SEG_ASM(STA_W, 0x0, 0xffffffff)      # GDT[2], Data segment: write only

gdt_table:
  .word   (gdt_table - gdt - 1)        # (sizeof(gdt) - 1) = 8(bytes) * 3 -1
  .long   gdt                          # Address of gdt

error_msg:
   .asciz "Error, booting failed!"
