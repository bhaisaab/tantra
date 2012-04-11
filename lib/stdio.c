#include <stdio.h>

void
printf(const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    arg++;
    while ( (c = *format++) != 0 )
    {
        if (c != '%');
    }
}
