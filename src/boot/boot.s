global loader                               ; the entry symbol for ELF

extern kmain                                ; the entry symbol for C
extern __mbHeader

MAGIC_NUMBER  equ 0x1BADB002                ; define the magic number constant
PAGE_ALIGN    equ 1<<0                      ; Load kernel and modules on a page boundary
MEM_INFO      equ 1<<1                      ; Provide your kernel with memory info
FLAGS         equ PAGE_ALIGN | MEM_INFO     ; multiboot flags i.e. ELF
CHECKSUM      equ -(MAGIC_NUMBER + FLAGS)   ; calculate the checksum
                                            ; (magic number + checksum + flags should equal 0)

KSTACK_SIZE   equ 0x8000                    ; size of stack, 32kB

section .__mbHeader                         ; multiboot header
align 4
    dd MAGIC_NUMBER                         ; write the magic number to the machine code,
    dd FLAGS                                ; the flags,
    dd CHECKSUM                             ; and the checksum

loader:                                     ; the loader label (defined as entry point in linker script)

setup_kstack:
    mov esp, kernel_stack + KSTACK_SIZE     ; point esp to the start of the
                                            ; stack (end of memory area)

call_kmain:
    push ebx                                ; loads multiboot header location
    push eax                                ; loads multiboot magic 0x1BADB002
    call kmain                              ; this pushes ebp (base pointer) and sets esp
                                            ; then calls the kmain, which sets 0xBADA55 on eax
                                            ; it pops ebp and returns

.loop:
    jmp .loop                               ; loop forever

section .bss
align 4                                     ; align at 4 bytes
kernel_stack:                               ; label points to beginning of memory
    resb KSTACK_SIZE                        ; reserve stack for the kernel
