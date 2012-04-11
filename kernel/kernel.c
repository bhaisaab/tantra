#include <types.h>
#include <x86.h>
#include <multiboot.h>
#include <mmu.h>
#include <drivers.h>

#include <kdebug.h>

#include <stdio.h>
#include <string.h>

char* banner =
#include <banner.h>
;

pde_t entrypgdir[];  // For entry.S

// Boot page table used in entry.S and entryother.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.  
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES] = {
  // Map VA's [0, 4MB) to PA's [0, 4MB)
  [0] = (0) + PTE_P + PTE_W + PTE_PS,
  // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
  [KERNBASE>>PDXSHIFT] = (0) + PTE_P + PTE_W + PTE_PS,
};

int
kmain(unsigned long magic, unsigned long addr)
{
  tty_init();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
      kprintf("Booted by tantra", 0x8f);
    else
      kprintf("Booted by GRUB", 0x8f);

    kprintf(banner, 2);

    kprintf("Initializing GDT, IDT and drivers...\n\n", 2);
    kprintf("2", 2);
    kprintf("3", 3);
    kprintf("4", 4);
    kprintf("5", 5);
    kprintf("6", 6);
    kprintf("7", 7);
    kprintf("8", 8);
    kprintf("9", 9);
    kprintf("10", 10);
    kprintf("11", 11);
    kprintf("12", 12);
    kprintf("13", 13);
    kprintf("14", 14);
    kprintf("15", 15);
    kprintf("Z", 0x10 + 0x7);
    kprintf("Z", 0x20 + 0x7);
    kprintf("Z", 0x30 + 0x7);
    kprintf("Z", 0x40 + 0x7);
    kprintf("Z", 0x50 + 0x7);
    kprintf("Z", 0x60 + 0x7);
    kprintf("Z", 0x70 + 0x7);
    kprintf("Z", 0x80 + 0x7);
    kprintf("Z", 0x90 + 0x7);
    kprintf("Z", 0xa0 + 0x7);
    kprintf("Z", 0xb0 + 0x7);
    kprintf("Z", 0xc0 + 0x7);
    kprintf("Z", 0xd0 + 0x7);
    kprintf("Z", 0xe0 + 0x7);
    kprintf("Z", 0xf0 + 0x7);

    /* Initialize descriptor tables: gdt and idt */

    /* Intialize display and input */

    /* Initialize memory */

    /* Initialize interrupt */

    /* Initialize process */

    for(;;);
    return 0xDEADBABA;
}
