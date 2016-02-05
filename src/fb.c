#include "fb.h"
#include "stdint.h"

const char* banner =
#include <banner.h>
;

static u16int *FB = (u16int *) FB_ADDRESS;
static u16int s_fbx = 0;
static u16int s_fby = 0;

static u16int s_attribute = 0x0200;

static inline u16int fb_attribute_opts(u8int bg, u8int fg)
{
    return ((((bg & 0x0f) << 4) | (fg & 0x0f)) << 8) & 0xff00;
}

static inline u16int fb_blank()
{
   return ' ' | s_attribute;
}

static inline u16int fb_position()
{
   return (s_fby * FB_COLUMNS) + s_fbx;
}

static inline void fb_move_cursor()
{
    u16int location = fb_position();
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((location >> 8) & 0x00ff));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    location & 0x00ff);
}

static inline void fb_scroll()
{
    if (s_fby >= FB_ROWS)
    {
        u16int blank = fb_blank();
        for (int i = 0; i < FB_COLUMNS * (FB_ROWS - 1); i++)
        {
            FB[i] = FB[i+80];
        }

        for (int i = FB_COLUMNS * (FB_ROWS - 1); i < FB_COLUMNS * FB_ROWS; i++)
        {
            FB[i] = blank;
        }
        s_fby = (FB_ROWS - 1);
    }
}

void fb_put(char c)
{
    u16int location = fb_position();

    if (c == 0x08 && s_fbx)
    {
        s_fbx--;
    }
    else if (c == '\r')
    {
        s_fbx = 0;
    }
    else if (c == '\n')
    {
        s_fbx = 0;
        s_fby++;
    }
    else if (c >= ' ')
    {
        FB[location] = c | s_attribute;
        s_fbx++;
    }

    if (s_fbx >= FB_COLUMNS)
    {
        s_fbx = 0;
        s_fby++;
    }
    fb_scroll();
    fb_move_cursor();
}

void fb_clear()
{
    u16int blank  = fb_blank();
    for (int i = 0; i < FB_COLUMNS * FB_ROWS; i++)
    {
        FB[i] = blank;
    }
    s_fbx = 0;
    s_fby = 0;
}

void fb_print(const char *c)
{
    int i = 0;
    while (c[i] != '\0')
    {
        fb_put(c[i++]);
    }
}

void fb_printcolor(const char* str, u8int bg, u8int fg)
{
    u16int original_attribute = s_attribute;
    s_attribute = fb_attribute_opts(bg, fg);
    fb_print(str);
    s_attribute = original_attribute;
}

void fb_putcolor(char c, u8int bg, u8int fg)
{
    u16int original_attribute = s_attribute;
    s_attribute = fb_attribute_opts(bg, fg);
    fb_put(c);
    s_attribute = original_attribute;
}

void fb_init()
{
    fb_clear();
    for (int idx = 0; banner[idx] != '\0'; idx++)
    {
        char c = banner[idx];
        if (c == '\0') return;
        int bg = FB_DGRAY;
        int fg = FB_LGRAY;

        if (c == 'w')
        {
            bg = FB_LMAGENTA;
            fg = FB_BROWN;
        }
        fb_putcolor(c, bg, fg);
    }
    fb_printcolor("Welcome to Tantra. Type h or ? to get started...\n", FB_LCYAN, FB_BLACK);
}

