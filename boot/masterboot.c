#include <types.h>
#include <elf.h>
#include <x86.h>

#define SECTSIZE  512
#define PAGESIZE  4096

void read_segment(uint8_t*, uint32_t, uint32_t);

void
masterboot(void)
{
  struct elfhdr *elf;
  struct proghdr *ph, *eph;
  void (*entry)(void);
  uint8_t* pa;

  elf = (struct elfhdr*) 0x10000; // scratch space

  // Read first page off disk
  read_segment((uint8_t*) elf, PAGESIZE, 0);

  // Is this a valid ELF?
  if (elf->magic != ELF_MAGIC)
    return;

  // Load each program segment (ignores ph flags)
  ph = (struct proghdr *) ((uint8_t*) elf + elf->phoff);
  eph = ph + elf->phnum;
  for (; ph < eph; ph++) {
    pa = (uint8_t*) ph->paddr;
    read_segment(pa, ph->filesz, ph->off);
    if (ph->memsz > ph->filesz)
      stosb(pa + ph->filesz, 0, ph->memsz - ph->filesz);
  }

  // Call the entry point from the ELF header
  entry = (void(*)(void))(elf->entry);
  entry();
}

void
waitdisk(void)
{
  // wait for disk reaady
  while ((inb(0x1F7) & 0xC0) != 0x40);
}

// Read a single sector at offset into dst
void
read_sector(void *dst, uint32_t offset)
{
  // wait for disk to be ready
  waitdisk();

  // Scan IDE controller for hard disk
  outb(0x1F2, 1); // count = 1
  outb(0x1F3, offset);
  outb(0x1F4, offset >> 8);
  outb(0x1F5, offset >> 16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20); // cmd 0x20 - read sectors

  // wait for disk to be ready
  waitdisk();

  // read a sector
  insl(0x1F0, dst, SECTSIZE/4);
}

// Read 'count' bytes at 'offset' from kernel into physical address 'pa'.
// Might copy more than asked.
void
read_segment(uint8_t* pa, uint32_t count, uint32_t offset)
{
  uint8_t* epa;
  epa = pa + count;

  // round down to sector boundary
  pa -= offset % SECTSIZE;

  // translate from bytes to sectors, and kernel starts at sector 1
  offset = (offset / SECTSIZE) + 1;

  // If this is too slow, we could read lots of sectors at a time.
  // We'd write more to memory than asked, but it doesn't matter --
  // we load in increasing order.
  for (; pa < epa; pa += SECTSIZE, offset++)
    read_sector(pa, offset);
}
