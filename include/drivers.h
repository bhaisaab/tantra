#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include <types.h>

#define VGA_MEM     0xB8000
#define MONO_MEM    0xB0000

#define VGA_BASE    0x3D4
#define MONO_BASE   0x3B4 

#define CRT_ROWS    25
#define CRT_COLS    80
#define CRT_SIZE    (CRT_ROWS * CRT_COLS)

void tty_init(void);

void vga_init(void);
void kbd_init(void);
void serial_init(void);

#endif /* _DRIVERS_H_ */
