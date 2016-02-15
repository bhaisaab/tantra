#include <timer.h>
#include <isr.h>
#include <fb.h>

static volatile uint32_t tick = 1;

static void timer_callback(registers_t regs)
{
    tick++;
    if (tick % 100 == 0) {
        fb_update_timer(tick / 100);
    }
    // TODO: scheduler
}

void init_timer(uint32_t frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193182 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xff);
   uint8_t h = (uint8_t)((divisor>>8) & 0xff);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}
