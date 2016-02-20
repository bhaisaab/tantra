#include <ctype.h>

int toupper(int ch)
{
    if ((unsigned int)(ch - 'a') < 26u) ch += 'A' - 'a';
    return ch;
}

int tolower(int ch)
{
    if ((unsigned int)(ch - 'A') < 26u) ch += 'a' - 'A';
    return ch;
}
