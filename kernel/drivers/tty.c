#include <drivers.h>

void vga_init(void);
void vga_putc(char, int);

void kbd_init(void);
void serial_init(void);

void
tty_putc(char c, int attribute)
{
  vga_putc(c, attribute);
}

void
tty_init()
{
  vga_init();
  kbd_init();
  serial_init();
}

