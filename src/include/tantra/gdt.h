#ifndef GDT_H
#define GDT_H

#include "stdint.h"

struct gdt_entry {
    uint16_t limit_low;     /* The lower 16 bits of the limit */
    uint16_t base_low;      /* The lower 16 bits of the base */
    uint8_t  base_middle;      /* The next 8 bits of the base */
    uint8_t  access;        /* Contains access flags */
    uint8_t  granularity;   /* Specify granularity, and 4 bits of limit */
    uint8_t  base_high;     /* The last 8 bits of the base; */
} __attribute__((packed));  /* It needs to be packed like this, 64 bits */
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr {
    uint16_t limit;              /* Size of gdt table in bytes*/
    uint32_t base;               /* Address to the first gdt entry */
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

extern void gdt_flush(uint32_t gtd_ptr);

#endif
