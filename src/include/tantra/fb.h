#ifndef FB_H
#define FB_H

#include "io.h"

// Framebuffer
// Memory layout:
// Bit:     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
// Content: | ASCII                 | FG      | BG      |

#define FB_ADDRESS    0x000b8000
#define FB_COLUMNS    80
#define FB_ROWS       25

// The I/O ports
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

// The I/O port commands
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

// Framebuffer colours
#define FB_BLACK      0x0
#define FB_BLUE       0x1
#define FB_GREEN      0x2
#define FB_CYAN       0x3
#define FB_RED        0x4
#define FB_MAGENTA    0x5
#define FB_BROWN      0x6
#define FB_LGRAY      0x7
#define FB_DGRAY      0x8
#define FB_LBLUE      0x9
#define FB_LGREEN     0xa
#define FB_LCYAN      0xb
#define FB_LRED       0xc
#define FB_LMAGENTA   0xd
#define FB_LBROWN     0xe
#define FB_WHITE      0xf

void init_fb();

void fb_clear();
void fb_put(char c);

void fb_update_timer(uint32_t tick);

#endif
