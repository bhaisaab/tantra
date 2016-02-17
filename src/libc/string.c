#include <stdint.h>
#include <string.h>

size_t strlen(const char *str)
{
    size_t size = 0;
    while (str[size++] != '\0');
    return size;
}

void memset(void* ptr, char chr, uint32_t size)
{
    uint8_t *p = (uint8_t*) ptr;
    while (size > 0 && size--) {
        *p++ = chr;
    }
}
