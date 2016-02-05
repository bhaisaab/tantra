#include "grub/multiboot.h"

int kmain(unsigned long magic, multiboot_header_t *mboot_header)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Something weird happened, Grub did not boot us
        return 0xD15EA5E;
    }

    // 0xBADA55 now set on eax/rax register from C
    return 0xBADA55;
}
