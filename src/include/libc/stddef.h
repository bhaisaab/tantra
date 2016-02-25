#ifndef STDDEF_H
#define STDDEF_H

typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;

typedef unsigned int   size_t;

typedef unsigned char  bool;

#define TRUE    1
#define FALSE   0

#define NULL    ((void *)0)

#define UNUSED(x) (void)(x)

#endif
