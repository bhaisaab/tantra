#include <kdebug.h>
#include <drivers.h>
#include <stdarg.h>
#include <stdio.h>

static void
kputc(char ch, int dummyfd)
{
  tty_putc(ch);
}

int
kprintf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
  int ret = vsprintf(kputc, -1, fmt, args);
	va_end(args);
  return ret;
}
