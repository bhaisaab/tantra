#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <stddef.h>

#define stdin   0
#define stdout  1
#define stderr  2

uint32_t vsprintf(void (*)(char, int32_t), uint32_t, const char *, va_list);

uint32_t fprintf(uint32_t, const char *, ...);
uint32_t sprintf(char *, const char *, ...);
uint32_t printf(const char *, ...);

#endif
