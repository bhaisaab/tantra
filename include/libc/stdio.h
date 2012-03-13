#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include <types.h>
#include <string.h>
#include <stdarg.h>

#ifndef NULL
#define NULL	((void *) 0)
#endif

void printf(char *, ...);

#endif
