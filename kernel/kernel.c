#include <types.h>
#include <x86.h>
#include <string.h>
#include <multiboot.h>
#include <mmu.h>
#include <drivers.h>

#include <stdio.h>
/* Screen */
#define COLUMNS   80
#define LINES     24

/* The video memory address.  */
#define VIDEO     0xB8000

/* To save the X position.  */
static int xpos;
/* To save the Y position.  */
static int ypos;

/* Point to the video memory.  */
static unsigned char *video;

void cls(void);
void updateScreen(void);
void putchar(char, int);

char* banner =
#include <banner.h>
;

pde_t entrypgdir[];  // For entry.S

// Boot page table used in entry.S and entryother.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.  
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES] = {
  // Map VA's [0, 4MB) to PA's [0, 4MB)
  [0] = (0) + PTE_P + PTE_W + PTE_PS,
  // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
  [KERNBASE>>PDXSHIFT] = (0) + PTE_P + PTE_W + PTE_PS,
};

int
kmain(unsigned long magic, unsigned long addr)
{
  tty_init();
  cls();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
      printf("Booted by tantra", 0x8f);
    else
      printf("Booted by GRUB", 0x8f);

    int i;
    for(i = 0; i < strlen(banner); i++)
      putchar(banner[i], 2);

    printf("Initializing GDT, IDT and drivers...\n\n", 2);
    printf("2", 2);
    printf("3", 3);
    printf("4", 4);
    printf("5", 5);
    printf("6", 6);
    printf("7", 7);
    printf("8", 8);
    printf("9", 9);
    printf("10", 10);
    printf("11", 11);
    printf("12", 12);
    printf("13", 13);
    printf("14", 14);
    printf("15", 15);
    printf("Z", 0x10 + 0x7);
    printf("Z", 0x20 + 0x7);
    printf("Z", 0x30 + 0x7);
    printf("Z", 0x40 + 0x7);
    printf("Z", 0x50 + 0x7);
    printf("Z", 0x60 + 0x7);
    printf("Z", 0x70 + 0x7);
    printf("Z", 0x80 + 0x7);
    printf("Z", 0x90 + 0x7);
    printf("Z", 0xa0 + 0x7);
    printf("Z", 0xb0 + 0x7);
    printf("Z", 0xc0 + 0x7);
    printf("Z", 0xd0 + 0x7);
    printf("Z", 0xe0 + 0x7);
    printf("Z", 0xf0 + 0x7);

    /* Initialize descriptor tables: gdt and idt */

    /* Intialize display and input */

    /* Initialize memory */

    /* Initialize interrupt */

    /* Initialize process */

    for(;;);
    return 0xDEADBABA;
}

/* Clear the screen and initialize VIDEO, XPOS and YPOS. */
void
cls(void)
{
    register int i;
    video = (unsigned char *) VIDEO;
    for( i = 0; i < COLUMNS * LINES * 2; i++ )
        *(video + i) = 0;
    xpos = 0;
    ypos = 0;
}

void
updateScreen(void)
{
    xpos = 0;
    ypos++;
    if( ypos >= LINES )
        ypos = 0;
    return;
}

/* Put the character C on the screen.  */
void
putchar(char c, int attribute)
{
    if( c == '\n' || c == '\r' )
        return updateScreen();

    *(video + (xpos + ypos * COLUMNS) * 2)     = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = attribute;

    xpos++;
    if (xpos >= COLUMNS)
        updateScreen();
}
