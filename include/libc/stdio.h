#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include <types.h>
#include <string.h>
#include <stdarg.h>

#ifndef NULL
#define NULL	((void *) 0)
#endif

#define stdin    0
#define stdout   1
#define stderr   2

int vsprintf(void (*)(char, int), int, const char *, va_list);
int sprintf(char *, const char *, ...);

int printf(const char *, ...);
int fprintf(int, const char *, ...);

#endif
