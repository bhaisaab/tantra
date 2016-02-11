#include <paging.h>
#include <stdint.h>
#include <fb.h>
#include <isr.h>

#define PAGE_SIZE 0x1000 // 4kB

extern kernel_end;

uint32_t placement_address = (uint32_t)&kernel_end;

uint32_t* frames;
uint32_t nframes;

page_directory_t* kernel_directory = 0;
page_directory_t* current_directory = 0;

void memset(void* ptr, uint8_t chr, uint32_t size)
{
    uint8_t *p = (uint8_t*) ptr;
    while (size > 0 && size--) {
        *p++ = chr;
    }
}

uint32_t kmalloc_internal(uint32_t sz, uint8_t align, uint32_t *phys)
{
    // Align address on 4096 bytes boundary
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    // Allocate physical memory
    if (phys) {
      *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_internal(sz, 0, 0);
}

uint32_t kmalloc_aligned(uint32_t sz)
{
    return kmalloc_internal(sz, 1, 0);
}

uint32_t kmalloc_alignedp(uint32_t sz, uint32_t *phys)
{
    return kmalloc_internal(sz, 1, phys);
}

void alloc_frame(page_t *page, uint8_t is_kernel, uint8_t is_writeable)
{
    // TODO: implement paging algorithm
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    __asm__ volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
   uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
    // Enable paging
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, uint8_t make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= PAGE_SIZE;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;
    // If this table is already assigned
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address%1024];
    } else if (make) {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_alignedp(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    return 0;
}

void page_fault(registers_t regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    fb_print("Page fault! ( ");
    if (present) {fb_print("present ");}
    if (rw) {fb_print("read-only ");}
    if (us) {fb_print("user-mode ");}
    if (reserved) {fb_print("reserved ");}
    fb_print(") at ");
    fb_puthex(faulting_address);
    fb_print("\n");
    fb_print("PAGE FAULT PANIC");
    for(;;);
}

void init_paging()
{
    // TODO: get physical memory size
    uint32_t total_physical_memory = 0x1000000; // Assume 16MiB for now
    nframes = total_physical_memory / PAGE_SIZE;
    frames = (uint32_t*) kmalloc(nframes);
    memset(frames, 0, nframes);

    kernel_directory = (page_directory_t*)kmalloc_aligned(sizeof(page_directory_t));
    current_directory = kernel_directory;

    fb_print("\nPaging Placement address = ");
    fb_puthex(placement_address);

    for (uint32_t i = 0; i < placement_address; i += PAGE_SIZE) {
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    }

    // Register page fault handler
    register_interrupt_handler(14, page_fault);

    // FIXME: with no frames allocated enabling paging causes double page fault
    // ... a good way to force restarts though :)
    return;

    // Enable paging
    switch_page_directory(kernel_directory);
}


