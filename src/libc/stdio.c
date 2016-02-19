#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

// FIXME: remove this when syscalls work
#include <fb.h>

#define UNSIGNED 0
#define SIGNED   1


static void putc(char ch, int32_t fd)
{
    if (fd == stdout) {
        fb_put(ch);
    }
}

static uint32_t __print_num(void (*putch)(char, int32_t), uint32_t fd, int32_t num, uint16_t base, bool sign)
{
    static char* digits = "0123456789abcdef";
    char buf[16];
    int32_t n = num;
    bool is_negative = FALSE;

    if (sign && num < 0) {
        n = -n;
        is_negative = TRUE;
    }

    int32_t idx = 0;
    do {
        buf[idx++] = digits[n % base];
        n /= base;
    } while (n > 0);

    if (is_negative) {
        buf[idx] = '-';
    }

    uint32_t ret = idx;
    while (idx >= 0) {
        putch(buf[idx--], fd);
    }
    return ret;
}

uint32_t vsprintf(void (*putch)(char, int32_t), uint32_t fd, const char *fmt, va_list args)
{
    int32_t count = 0;
    char *substr;
    for (int32_t idx = 0; fmt[idx] != '\0'; idx++) {
        char c = fmt[idx] & 0xff;
        bool sign = UNSIGNED;
        if (c != '%') {
            putch(c, fd);
            count++;
            continue;
        }
        c = fmt[++idx] & 0xff;;
        switch (c) {
        case 'c':
            putch(va_arg(args, char), fd);
            count++;
            break;
        case 'd':
        case 'i':
            sign |= SIGNED;
        case 'u':
            count += __print_num(putch, fd, va_arg(args, int), 10, sign);
            break;
        case 'l':
            sign |= SIGNED;
            count += __print_num(putch, fd, va_arg(args, long), 10, sign);
            break;
        case 'x':
            putch('0', fd);
            putch('x', fd);
            count += 2;
            count += __print_num(putch, fd, va_arg(args, long), 16, UNSIGNED);
            break;
        case 's':
            for (substr = va_arg(args, char*); *substr != '\0'; substr++, count++) {
                putch(*substr, fd);
            }
            break;
        case '%':
            putch('%', fd);
            idx++;
            count++;
            break;
        default:
            putch('%', fd);
            putch(c, fd);
            idx += 2;
            count += 2;
        }
    }
    return count;
}

static char* __buf_ptr;

static void __putch_buf(char ch, int fd)
{
    UNUSED(fd);
    *__buf_ptr++ = ch;
}

uint32_t sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __buf_ptr = buf;
    uint32_t ret = vsprintf(__putch_buf, -1, fmt, args);
    *__buf_ptr = '\0';
    va_end(args);
    return ret;
}

uint32_t fprintf(uint32_t fd, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    uint32_t ret = vsprintf(putc, fd, fmt, args);
    va_end(args);
    return ret;
}

uint32_t printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    uint32_t ret = vsprintf(putc, stdout, fmt, args);
    va_end(args);
    return ret;
}
