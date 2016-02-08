#include <grub/multiboot.h>
#include <stdint.h>
#include <io.h>
#include <fb.h>
#include <descriptor_table.h>

uint32_t kmain(unsigned long magic, multiboot_header_t *mboot_header)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Something weird happened, Grub did not boot us
        return 0xD15EA5E;
    }

    init_descriptor_tables();

    fb_init();
    fb_print("> ");

    __asm__("int $0x03");
    __asm__("int $0x04");

    // 0xBADA55 now set on eax/rax register from C
    return 0xBADA55;
}
