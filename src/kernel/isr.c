#include <stdint.h>
#include <isr.h>
#include <fb.h>

#include <kdebug.h>

const char* exception_messages[] = {
    "Division By Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception (486+)",
    "Machine Check Exception (Pentium/586+)"
};

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs)
{
    kprintf("\n[Interrupt] code=%d", regs.int_no);
    if (regs.int_no < 20) {
        kprintf("\n[Interrupt] message=%s", exception_messages[regs.int_no]);
    }

   if (interrupt_handlers[regs.int_no] != 0) {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

void irq_handler(registers_t regs)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40) {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0) {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}
