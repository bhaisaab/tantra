#include <drivers.h>
#include <mmu.h>
#define COLOR_FRAMEBUFFER  0xB8000
#define MONO_FRAMEBUFFER   0xB0000

#define VGA_BASE    0x3D4
#define MONO_BASE   0x3B4

#define CRT_ROWS    25
#define CRT_COLS    80
#define CRT_SIZE    (CRT_ROWS * CRT_COLS)

static int xpos;
static int ypos;
static unsigned char *video;

void
vga_cls(void)
{
  register int i;
  video = (unsigned char*) COLOR_FRAMEBUFFER+KERNBASE;
  for(i = 0; i < CRT_SIZE * 2; i++)
    *(video + i) = 0;
  xpos = 0;
  ypos = 0;
}

void
vga_scroll(void)
{
  xpos = 0;
  ypos++;
  if(ypos > CRT_ROWS)
    ypos = 0;
}

void
vga_putc(char c, int attribute)
{
  if(c == '\n' || c == '\r')
    return vga_scroll();
  *(video + (xpos + ypos * CRT_COLS) * 2)     = c & 0xFF;
  *(video + (xpos + ypos * CRT_COLS) * 2 + 1) = attribute;

  xpos++;
  if(xpos > CRT_ROWS)
    vga_scroll();
}

void
vga_init()
{
  vga_cls();
}
