global outb         ; for sending a byte to a port
global inb          ; for reading a byte from a port
global inw          ; for reading a word from port

section .text:

outb:                    ; outb - send a byte to an I/O port
    mov al, [esp + 8]    ; move the data to be sent into the al register
    mov dx, [esp + 4]    ; move the address of the I/O port into the dx register
    out dx, al           ; send the data to the I/O port
    ret                  ; return to the calling function

inb:
    mov dx, [esp+4] ; the port to read from
    in  al, dx      ; reads a byte from dx into al
    ret             ; returns the read byte

inw:
    mov dx, [esp+4] ; the port to read from
    in  ax, dx      ; read a word from dx into ax
    ret             ; returns the read word
