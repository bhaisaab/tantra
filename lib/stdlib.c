#include <stdlib.h>

int
atoi(const char *s)
{
  int n = 0;
  while (*s >= '0' && *s <= '9') n = n * 10 + *s++ - '0';
  return n;
}
