#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include <types.h>

int strlen(const char *);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);

char * strcpy(char *, const char *);
char * strncpy(char *, const char *, size_t);
char * strchr(const char *, char);

void * memset(void *, int, size_t);
void * memcpy(void *, void *, size_t);

int memcmp(const void *, const void *, size_t);

#endif

