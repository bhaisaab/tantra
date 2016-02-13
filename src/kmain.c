#include <grub/multiboot.h>
#include <descriptor_table.h>
#include <stdint.h>
#include <io.h>
#include <fb.h>
#include <timer.h>
#include <keyboard.h>
#include <paging.h>

uint32_t kmain(unsigned long magic, multiboot_info_t *mboot)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Something weird happened, Grub did not boot us
        return 0xD15EA5E;
    }

    // Intialize framebuffer
    init_fb();

    fb_print("\nGrub magic = ");
    fb_puthex(magic);

    if (mboot->flags & (1 << 2)) {
        fb_print("\nGrub cmdline = ");
        fb_print(mboot->cmdline);
    }

    fb_print("\nGrub multiboot flags = ");
    fb_puthex(mboot->flags);

    fb_print("\nMemory Lower = ");
    fb_puthex(mboot->mem_lower);

    fb_print("\nMemory Upper = ");
    fb_puthex(mboot->mem_upper);

    uint32_t availableMemoryKB = 0;
    multiboot_memory_map_t *mmap = mboot->mmap_addr;
    while (mmap < (mboot->mmap_addr + mboot->mmap_length)) {
        fb_print("\n  mmap mem_type = ");
        fb_puthex(mmap->type);
        fb_print(" mem_size = ");
        fb_putdec(mmap->len / 1024);
        fb_print("kB mem_address = ");
        fb_puthex(mmap->addr);
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            availableMemoryKB += mmap->len / 1024;
        }
        mmap = (multiboot_memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }

    fb_print("\nAvailable Physical Memory (MB) = ");
    fb_putdec(availableMemoryKB / 1024);

    // Initialize descriptor tables
    init_descriptor_tables();

    // Initialize timer at 100hz
    init_timer(100);

    // Initialize keyboard
    init_keyboard();

    // Initialize paging
    init_paging();

    // Enable interrupts
    __asm__("sti");

    // Dummy interrupts to test ISRs
    //__asm__("int $0x03");
    //__asm__("int $0x04");

    // Test page fault
    //uint32_t *ptr = (uint32_t*) 0xDEADFA11;
    //uint32_t do_page_fault = *ptr;

    // 0xBADA55 now set on eax/rax register from C
    return 0xBADA55;
}
