#include <x86.h>
#include <elf.h>

#define SECTSIZE  512
#define PAGESIZE  4096
#define ELFHDR    ((struct Elf *) 0x10000) // scratch space

void read_sector(void *addr, uint32_t sectornum);
void read_segment(uint32_t va, uint32_t filesz, uint32_t memsz, uint32_t offset);

void
masterboot(void)
{
    struct Proghdr *ph, *eph;

    // read 1st page off disk
    read_segment((uint32_t) ELFHDR, PAGESIZE, PAGESIZE, 0);

    // is this a valid ELF?
    if (ELFHDR->e_magic != ELF_MAGIC)
        goto bad;

    // load each program segment (ignores ph flags)
    ph = (struct Proghdr *) ((uint8_t *) ELFHDR + ELFHDR->e_phoff);
    eph = ph + ELFHDR->e_phnum;
    for (; ph < eph; ph++)
        read_segment(ph->p_va, ph->p_filesz, ph->p_memsz, ph->p_offset);

    // call the entry point from the ELF header
    // note: does not return!
    ((void (*)(void)) (ELFHDR->e_entry & 0xFFFFFF))();
bad:
    outw(0x8A00, 0x8A00);
    outw(0x8A00, 0x8E00);
    /* boot.S will spin for us */
}

// Read 'filesz' bytes at 'offset' from kernel into virtual address 'va',
// then clear the memory from 'va+filesz' up to 'va+memsz' (set it to 0).
void
read_segment(uint32_t va, uint32_t filesz, uint32_t memsz, uint32_t offset)
{
    uint32_t end_va;

    va &= 0xFFFFFF;
    end_va = va + filesz;
    memsz += va;

    // round down to sector boundary
    va &= ~(SECTSIZE - 1);

    // translate from bytes to sectors, and kernel starts at sector 1
    offset = (offset / SECTSIZE) + 1;

    // If this is too slow, we could read lots of sectors at a time.
    // We'd write more to memory than asked, but it doesn't matter --
    // we load in increasing order.
    while (va < end_va) {
        read_sector((uint8_t*) va, offset);
        va += SECTSIZE;
        offset++;
    }

    // clear bss segment
    while (end_va < memsz)
        *((uint8_t*) end_va++) = 0;
}

void
waitdisk(void)
{
    // wait for disk reaady
    while ((inb(0x1F7) & 0xC0) != 0x40);
}

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

