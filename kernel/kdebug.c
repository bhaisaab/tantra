#include <kdebug.h>
#include <drivers.h>
#include <stdarg.h>
void
kprintf(const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  char** arg = (char **) &format;
  int c;
  arg++;
  while((c = *format++) != 0)
  {
    if(c != '%')
      tty_putc(c, va_arg(ap, int));
  }
}
