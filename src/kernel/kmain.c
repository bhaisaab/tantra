#include <grub/multiboot.h>
#include <descriptor_table.h>
#include <io.h>
#include <fb.h>
#include <timer.h>
#include <keyboard.h>
#include <paging.h>

#include <kdebug.h>

extern uint32_t loader;

uint32_t kmain(unsigned long magic, multiboot_info_t *mboot)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        return 0xD15EA5E;
    }

    // Intialize framebuffer
    init_fb();

    kprintf("\nKernel loading linear address = %x", loader);
    kprintf("\nGrub magic = %x", magic);

    if (mboot->flags & (1 << 2)) {
        kprintf("\nGrub cmdline = %s", (const char*)mboot->cmdline);
    }

    uint32_t availableMemory = 0;
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mboot->mmap_addr;
    while (mmap < (multiboot_memory_map_t*)(mboot->mmap_addr + mboot->mmap_length)) {
        kprintf("\n  memmap type=%x size=%xkb start_addr=%x end_addr=%x", mmap->type, mmap->len/1024, (uint32_t) mmap->addr, (uint32_t) mmap->addr + mmap->len);
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            availableMemory += mmap->len;
        }
        mmap = (multiboot_memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }
    kprintf("\nAvailable Physical Memory = %x kB", availableMemory);

    // Initialize descriptor tables
    init_descriptor_tables();

    // Initialize timer at 100hz
    init_timer(100);

    // Initialize keyboard
    init_keyboard();

    // Initialize paging
    init_paging(availableMemory);

    // Enable interrupts
    __asm__("sti");

    kprintf("\n> ");

    // Dummy interrupts to test ISRs
    //__asm__("int $0x03");
    //__asm__("int $0x04");

    // Test page fault
    //uint32_t *ptr = (uint32_t*) 0xDEADFA11;
    //uint32_t do_page_fault = *ptr;

    // 0xBADA55 now set on eax/rax register from C
    return 0xBADA55;
}
