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
    kprintf("Booted by tantra");
  else
    kprintf("Booted by GRUB");

  kprintf(banner);
  kprintf("kprintf test: %c %s %d %x %%\n", '@', "StRiNg", -1234567890, 0xdeadc0de);
  kprintf("machine sizes: int_8=%d int_16=%d int_32=%d int_64=%d\n", sizeof(int8_t), sizeof(int16_t), sizeof(int32_t), sizeof(int64_t));

  /* Initialize descriptor tables: gdt and idt */

  /* Intialize display and input */

  /* Initialize memory */

  /* Initialize interrupt */

  /* Initialize process */

  for(;;);
  return 0xDEADBABA;
}
