#ifndef MMU_H
#define MMU_H

// Eflags register
#define FL_CF           0x00000001      // Carry Flag
#define FL_PF           0x00000004      // Parity Flag
#define FL_AF           0x00000010      // Auxiliary carry Flag
#define FL_ZF           0x00000040      // Zero Flag
#define FL_SF           0x00000080      // Sign Flag
#define FL_TF           0x00000100      // Trap Flag
#define FL_IF           0x00000200      // Interrupt Enable
#define FL_DF           0x00000400      // Direction Flag
#define FL_OF           0x00000800      // Overflow Flag
#define FL_IOPL_MASK    0x00003000      // I/O Privilege Level bitmask
#define FL_IOPL_0       0x00000000      //   IOPL == 0
#define FL_IOPL_1       0x00001000      //   IOPL == 1
#define FL_IOPL_2       0x00002000      //   IOPL == 2
#define FL_IOPL_3       0x00003000      //   IOPL == 3
#define FL_NT           0x00004000      // Nested Task
#define FL_RF           0x00010000      // Resume Flag
#define FL_VM           0x00020000      // Virtual 8086 mode
#define FL_AC           0x00040000      // Alignment Check
#define FL_VIF          0x00080000      // Virtual Interrupt Flag
#define FL_VIP          0x00100000      // Virtual Interrupt Pending
#define FL_ID           0x00200000      // ID flag

// Control register flags
#define CR0_PE          0x00000001      // Protection Enable
#define CR0_MP          0x00000002      // Monitor coProcessor
#define CR0_EM          0x00000004      // Emulation
#define CR0_TS          0x00000008      // Task Switched
#define CR0_ET          0x00000010      // Extension Type
#define CR0_NE          0x00000020      // Numeric Errror
#define CR0_WP          0x00010000      // Write Protect
#define CR0_AM          0x00040000      // Alignment Mask
#define CR0_NW          0x20000000      // Not Writethrough
#define CR0_CD          0x40000000      // Cache Disable
#define CR0_PG          0x80000000      // Paging

#define CR4_PCE         0x00000100      // Performance counter enable
#define CR4_MCE         0x00000040      // Machine Check Enable
#define CR4_PSE         0x00000010      // Page Size Extensions
#define CR4_DE          0x00000008      // Debugging Extensions
#define CR4_TSD         0x00000004      // Time Stamp Disable
#define CR4_PVI         0x00000002      // Protected-Mode Virtual Interrupts
#define CR4_VME         0x00000001      // V86 Mode Extensions

// GDT Entries int the table as per index
#define SEG_KCODE       1               // kernel code
#define SEG_KDATA       2               // kernel data+stack
#define SEG_KCPU        3               // kernel per-cpu data
#define SEG_UCODE       4               // user code
#define SEG_UDATA       5               // user data+stack
#define SEG_TSS         6               // process's task state

// GDT structure: 8 bytes long
//  word(limit_low: The lower 16 bits of the limit)
//  word(base_low: The lower 16 bits of the base)
//  byte(base_middle: The next 8 bits of the base)
//  byte(access: Access flags, determine what ring this segment can be used in)
//  byte(granularity: bytes of kbytes)
//  byte(base_high: The last 8 bits of the base)

#define SEG_NULLASM       \
        .word 0, 0;       \
        .byte 0, 0, 0, 0

// The 0xC0 means the limit is in 4096-byte units
// and (for executable segments) 32-bit mode.
#define SEG_ASM(type,base,lim)                                  \
        .word (((lim) >> 12) & 0xffff), ((base) & 0xffff);      \
        .byte (((base) >> 16) & 0xff), (0x90 | (type)),         \
                (0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

// Segment flags
#define STA_X     0x8       // Executable segment
#define STA_E     0x4       // Expand down (non-executable segments)
#define STA_C     0x4       // Conforming code segment (executable only)
#define STA_W     0x2       // Writeable (non-executable segments)
#define STA_R     0x2       // Readable (executable segments)
#define STA_A     0x1       // Accessed

#endif /*MMU_H*/
