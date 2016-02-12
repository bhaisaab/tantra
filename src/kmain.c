#include <grub/multiboot.h>
#include <descriptor_table.h>
#include <stdint.h>
#include <io.h>
#include <fb.h>
#include <timer.h>
#include <keyboard.h>
#include <paging.h>

uint32_t kmain(unsigned long magic, multiboot_header_t *mboot_header)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Something weird happened, Grub did not boot us
        return 0xD15EA5E;
    }

    // Intialize framebuffer
    init_fb();

    fb_print("Entry address = ");
    fb_puthex(mboot_header->entry_addr);

    fb_print("\nLoad address = ");
    fb_puthex(mboot_header->load_addr);

    fb_print("\nLoad end address = ");
    fb_puthex(mboot_header->load_end_addr);

    fb_print("\nHeader address = ");
    fb_puthex(mboot_header->header_addr);

    fb_print("\nFlags = ");
    fb_puthex(mboot_header->flags);

    fb_print("\nMagic = ");
    fb_puthex(magic);

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
