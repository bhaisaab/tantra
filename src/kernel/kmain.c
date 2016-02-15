#include <grub/multiboot.h>
#include <descriptor_table.h>
#include <stdint.h>
#include <io.h>
#include <fb.h>
#include <timer.h>
#include <keyboard.h>
#include <paging.h>

extern uint32_t loader;

uint32_t kmain(unsigned long magic, multiboot_info_t *mboot)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Something weird happened, Grub did not boot us
        return 0xD15EA5E;
    }

    // Intialize framebuffer
    init_fb();

    fb_print("Kernel loading linear address = ");
    fb_puthex(loader);

    fb_print("\nGrub magic = ");
    fb_puthex(magic);

    if (mboot->flags & (1 << 2)) {
        fb_print("\nGrub cmdline = ");
        fb_print(mboot->cmdline);
    }

    uint32_t availableMemory = 0;
    multiboot_memory_map_t *mmap = mboot->mmap_addr;
    while (mmap < (mboot->mmap_addr + mboot->mmap_length)) {
        fb_print("\n  mmap type = ");
        fb_puthex(mmap->type);
        fb_print(" size = ");
        fb_putdec(mmap->len / 1024);
        fb_print("kB addr = ");
        fb_puthex(mmap->addr);
        fb_print("kB end_addr = ");
        fb_puthex(mmap->addr + mmap->len);
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            availableMemory += mmap->len;
        }
        mmap = (multiboot_memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }

    fb_print("\nAvailable Physical Memory (kB) = ");
    fb_putdec(availableMemory / 1024);

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

    // Dummy interrupts to test ISRs
    //__asm__("int $0x03");
    //__asm__("int $0x04");

    // Test page fault
    //uint32_t *ptr = (uint32_t*) 0xDEADFA11;
    //uint32_t do_page_fault = *ptr;

    // 0xBADA55 now set on eax/rax register from C
    return 0xBADA55;
}
