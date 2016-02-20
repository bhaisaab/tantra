#ifndef STRING_H
#define STRING_H

size_t strlen(const char *str);

int strcmp(const char *p, const char *q);
int strncmp(const char *p, const char *q, size_t n);

char* strcpy(char *dst, const char *src);
char* strncpy(char *dst, const char *src, size_t size);
char* strchr(const char *s, char c);

void memset(void* ptr, char chr, uint32_t size);
int memcmp(const void *v1, const void *v2, size_t n);
void* memcpy(void *dst, void *src, size_t n);

#endif
