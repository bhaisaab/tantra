#ifndef STDARG_H
#define STDARG_H

typedef char* va_list;

#define __va_size(T)      (sizeof(T) / sizeof(char))

#define va_start(ap, T)   (ap = (va_list)&T + __va_size(T))

#define va_arg(ap, T)     (*(T*)(ap += __va_size(T), ap - __va_size(T)))

#define va_end(ap)        ((void)0)

#endif
