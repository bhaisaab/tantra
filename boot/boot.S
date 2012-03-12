# This code runs from the first sector of booting media into memory at
# physical address 0x7c00 and starts executing in real mode with
# %cs=0 %ip=7c00

.code16
.globl start
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
  orl     $0x1, %eax                   # Enable gate, 0x1
  movl    %eax, %cr0                   # Enter Protected Mode
  ljmp    $0x8, $protected_boot        # Far jump, but in 32-bit code segment

.code32                                # Assemble for 32-bit mode
protected_boot:
  movw    $0x10, %ax                   # Set protected mode segment registers
  movw    %ax, %ds
  movw    %ax, %es
  movw    %ax, %ss
  movw    %ax, %fs
  movw    %ax, %gs

  # Set up the stack pointer and call the C function
  movl    $start, %esp
  call    masterboot

spin:
  hlt
  jmp     spin

# Bootstrap GDT
# Structure: 8 byte long
#   word(limit_low: The lower 16 bits of the limit)
#   word(base_low: The lower 16 bits of the base)
#   byte(base_middle: The next 8 bits of the base)
#   byte(access: Access flags, determine what ring this segment can be used in)
#   byte(granularity: bytes of kbytes)
#   byte(base_high: The last 8 bits of the base)

.p2align 2                             # force 4 byte alignment
gdt:
  .word 0, 0                           # GDT[0]
  .byte 0, 0, 0, 0                     # Dummy

  .word 0xffff, 0x0000                 # GDT[1]
  .byte 0x00, 0x9a, 0xcf, 0x00         # Code Segment, read/write only

  .word 0xffff, 0x0000                 # GDT[2]
  .byte 0x00, 0x92, 0xcf, 0x00         # Data Segment, write only

gdt_table:
  .word   0x17                         # (sizeof(gdt) - 1) = 8(bytes) * 3 -1
  .long   gdt                          # Address of gdt
