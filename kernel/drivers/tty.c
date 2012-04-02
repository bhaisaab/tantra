#include <drivers.h>

void
tty_init()
{
  vga_init();
  kbd_init();
  serial_init();
}
