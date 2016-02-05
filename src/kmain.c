#include <grub/multiboot.h>
#include <stdint.h>
#include <io.h>
#include <fb.h>

int kmain(unsigned long magic, multiboot_header_t *mboot_header)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Something weird happened, Grub did not boot us
        return 0xD15EA5E;
    }

    fb_init();
    fb_print("> ");

    // 0xBADA55 now set on eax/rax register from C
    return 0xBADA55;
}
