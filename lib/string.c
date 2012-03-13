#include <string.h>

int
strlen(const char *s)
{
  int n;
  for (n=0; *s != '\0'; ++s) n++;
  return n;
}

int
strcmp(const char *p, const char *q)
{
  while (*p && *p == *q) ++p, ++q;
  return (int) ((unsigned char) *p - (unsigned char) *q);
}

int
strncmp(const char *p, const char *q, size_t n)
{
  while (n > 0 && *p && *p == *q) --n, ++p, ++q;
  if (n == 0) return 0;
  else return (int) ((unsigned char) *p - (unsigned char) *q);
}

char *
strcpy(char *dst, const char *src)
{
  char *ret = dst;
  while ((*dst++ = *src++) != '\0');
  return ret;
}

char *
strncpy(char *dst, const char *src, size_t size)
{
  char *ret = dst;
  size_t i;
  for (i = 0; i < size; ++i) {
    *dst++ = *src;
    if (*src != '\0') src++;
  }
  return ret;
}

char *
strchr(const char *s, char c)
{
  for (; *s; ++s)
    if (*s == c) return (char *) s;
  return (char *) 0;
}

void *
memset(void *v, int c, size_t n)
{
  char *p = v;
  int m = n;
  while (--m >= 0) *p++ = c;
  return v;
}

void *
memcpy(void *dst, void *src, size_t n)
{
  const char *s = src;
  char *d = dst;
  if (s < d && (s+n) > d) {
    s += n;
    d += n;
    while (n-- > 0) *--d = *--s;
  } else
    while (n-- > 0) *d++ = *s++;

  return dst;
}

int
memcmp(const void *v1, const void *v2, size_t n)
{
  const uint8_t *s1 = (const uint8_t *) v1;
  const uint8_t *s2 = (const uint8_t *) v2;

  while (n-- > 0) {
    if (*s1 != *s2) return (int) *s1 - (int) *s2;
    s1++, s2++;
  }

  return 0;
}

