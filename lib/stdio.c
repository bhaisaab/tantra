#include <stdio.h>
#include <drivers.h> // REMOVE ME when putc/write is fixed

#define UNSIGNED 0
#define SIGNED   1

static int
write(int fd, char c, int n)
{
  tty_putc(c);
  return 1;
}

static void
putc(char c, int fd)
{
  write(fd, c, 1); // TODO: impl write syscall
}

//FIXME: I'm unreliable
static int
printnum(void (*putch)(char, int), int fd, int xx, int base, int sgn)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i, neg, ret;
  uint32_t x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';
  ret = i;
  while(--i >= 0)
    putch(buf[i], fd);
  return ret;
}

int
vsprintf(void (*putch)(char, int), int fd, const char *fmt, va_list args)
{
  char *substr;
  int i, signedness = UNSIGNED, len;
  char ch;

  for (len = 0, i = 0; fmt[i];) {
    signedness = UNSIGNED;
    ch = *(fmt+i++) & 0xff;

    if (ch != '%') {
      putch(ch, fd);
      len++;
      continue;
    }
    ch = *(fmt+i++) & 0xff;
    switch (ch) {
    case 'c':
      putch(va_arg(args, char), fd);
      break;
    case 'd':
    case 'i':
      signedness |= SIGNED;
    case 'u':
      len += printnum(putch, fd, va_arg(args, int), 10, signedness);
      break;
    case 'l':
      signedness |= SIGNED;
      len += printnum(putch, fd, va_arg(args, long), 10, signedness);
      break;
    case 'x':
    case 'p':
      putch('0', fd);
      putch('x', fd);
      len += 2;
      len += printnum(putch, fd, va_arg(args, uint32_t), 16, UNSIGNED);
      break;
    case 's':
      for (substr = va_arg(args, char*); *substr != '\0'; len++, substr++)
        putch(*substr, fd);
      break;
    case '%':
      putch('%', fd);
      len++;
      break;
    default:
      putch('%', fd);
      putch(ch, fd);
      len += 2;
      break;
    }
  }
  return len;
}

static char* _bufPtr;

static void
_sprintfputch(char ch, int fd)
{
  *_bufPtr++ = ch;
}

int
sprintf(char* buf, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  _bufPtr = buf;
  int ret = vsprintf(_sprintfputch, -1, fmt, args);
  *_bufPtr = '\0';
  va_end(args);
  return ret+1;
}

int
printf(const char *fmt, ...)
{
  va_list args;
	va_start(args, fmt);
	int ret = vsprintf(putc, stdout, fmt, args);
	va_end(ap);
  return ret;
}

int
fprintf(int fd, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
  int ret = vsprintf(putc, fd, fmt, args);
	va_end(ap);
  return ret;
}
