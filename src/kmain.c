#include <grub/multiboot.h>
#include <descriptor_table.h>
#include <stdint.h>
#include <io.h>
#include <fb.h>
#include <timer.h>
#include <keyboard.h>

uint32_t kmain(unsigned long magic, multiboot_header_t *mboot_header)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Something weird happened, Grub did not boot us
        return 0xD15EA5E;
    }

    // Initialize descriptor tables
    init_descriptor_tables();

    // Intialize framebuffer
    init_fb();

    // Initialize timer at 100hz
    init_timer(100);

    // Initialize keyboard
    init_keyboard();

    // Enable interrupts
    __asm__("sti");

    // Dummy interrupts to test ISRs
    //__asm__("int $0x03");
    //__asm__("int $0x04");

    fb_print("> ");

    // 0xBADA55 now set on eax/rax register from C
    return 0xBADA55;
}
