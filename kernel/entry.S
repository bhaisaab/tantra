#include <multiboot.h>
#include <mmu.h>

#define KSTACKSIZE 0x4000
#define	RELOC(x) ((x) - KERNBASE)

.p2align 2
.text
.globl multiboot_header
multiboot_header:
  .long MULTIBOOT_HEADER_MAGIC         # The Multiboot header
  .long MULTIBOOT_HEADER_FLAGS
  .long CHECKSUM

.globl _start
_start = RELOC(entry)

.globl entry
entry:
  .code32
	movw    $0x1234,0x472                # Warm boot

  pushl   %ebx                         # Multiboot info struct
  pushl   %eax                         # Multiboot magic value

  # Turn on page size extension for 4Mbyte pages
  movl    %cr4, %eax
  orl     $(CR4_PSE), %eax
  movl    %eax, %cr4
  # Set page directory
  movl    $(RELOC(entrypgdir)), %eax
  movl    %eax, %cr3
  # Turn on paging.
  movl    %cr0, %eax
  orl     $(CR0_PG|CR0_WP), %eax
  movl    %eax, %cr0

  mov     $(stack + KSTACKSIZE), %esp  # Set up the stack

	mov     $relocated, %ecx
	jmp     *%ecx
relocated:

  pushl   $0                           # Reset eflags
  popf
  call    kmain                        # Call kernel's C entry point

spin:
  hlt
  jmp     spin

.data
.comm stack, KSTACKSIZE, 32            # Reserve 32k stack on a quadword boundary
