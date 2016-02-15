#ifndef ISR_H
#define ISR_H

#include <stdint.h>

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// Interrupt Service Routines
// #  Description                                 Error Code?
// 0  Division By Zero Exception                  No
// 1  Debug Exception                             No
// 2  Non Maskable Interrupt Exception            No
// 3  Breakpoint Exception                        No
// 4  Into Detected Overflow Exception            No
// 5  Out of Bounds Exception                     No
// 6  Invalid Opcode Exception                    No
// 7  No Coprocessor Exception                    No
// 8  Double Fault Exception                      Yes
// 9  Coprocessor Segment Overrun Exception       No
// 10 Bad TSS Exception                           Yes
// 11 Segment Not Present Exception               Yes
// 12 Stack Fault Exception                       Yes
// 13 General Protection Fault Exception          Yes
// 14 Page Fault Exception                        Yes
// 15 Unknown Interrupt Exception                 No
// 16 Coprocessor Fault Exception                 No
// 17 Alignment Check Exception (486+)            No
// 18 Machine Check Exception (Pentium/586+)      No
// 19 to 31  Reserved Exceptions No

typedef struct registers
{
    uint32_t ds;                                      // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by pusha.
    uint32_t int_no, err_code;                        // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;            // Pushed by the processor automatically.
} registers_t;

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

void isr_handler(registers_t regs);
void irq_handler(registers_t regs);

#endif
