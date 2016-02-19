#include <kdebug.h>
#include <fb.h>

static void kputc(char c, int ignore)
{
    UNUSED(ignore);
    fb_put(c);
}

void kprintf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(kputc, -1, fmt, args);
    va_end(args);
}

