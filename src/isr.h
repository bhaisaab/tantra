#ifndef ISR_H
#define ISR_H

#include <stdint.h>

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

#endif
