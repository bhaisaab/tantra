#include <ctype.h>
#include <stddef.h>

int isspace(int ch)
{
    if ((unsigned char)ch == ' ') return TRUE;
    return FALSE;
}

int isalpha(int ch)
{
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) return TRUE;
    return FALSE;
}

int isdigit(int ch)
{
    if (ch >= '0' && ch <= '9') return TRUE;
    return FALSE;
}

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
