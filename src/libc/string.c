#include <stddef.h>
#include <string.h>

size_t strlen(const char *str)
{
    size_t size = 0;
    while (str[size++] != '\0');
    return size;
}

int strcmp(const char *p, const char *q)
{
    while (*p && *p == *q) ++p, ++q;
    return (int) ((unsigned char) *p - (unsigned char) *q);
}

int strncmp(const char *p, const char *q, size_t n)
{
    while (n > 0 && *p && *p == *q) --n, ++p, ++q;
    if (n == 0) return 0;
    else return (int) ((unsigned char) *p - (unsigned char) *q);
}

char* strcpy(char *dst, const char *src)
{
    char *ret = dst;
    while ((*dst++ = *src++) != '\0');
    return ret;
}

char* strncpy(char *dst, const char *src, size_t size)
{
    char *ret = dst;
    for (size_t i = 0; i < size; ++i) {
        *dst++ = *src;
        if (*src != '\0') src++;
    }
    return ret;
}

char* strchr(const char *s, char c)
{
    for (; *s; ++s) {
        if (*s == c) return (char *) s;
    }
    return (char *) 0;
}

void memset(void* ptr, char chr, uint32_t size)
{
    uint8_t *p = (uint8_t*) ptr;
    while (size > 0 && size--) {
        *p++ = chr;
    }
}

void* memcpy(void *dst, void *src, size_t n)
{
    const uint8_t *s = src;
    uint8_t *d = dst;
    while (n-- > 0) {
        *d++ = *s++;
    }
    return dst;
}

int memcmp(const void *v1, const void *v2, size_t n)
{
  const uint8_t *s1 = (const uint8_t *) v1;
  const uint8_t *s2 = (const uint8_t *) v2;

  while (n-- > 0) {
    if (*s1 != *s2) return (int) *s1 - (int) *s2;
    s1++, s2++;
  }

  return 0;
}

