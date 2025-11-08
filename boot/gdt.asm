gdt_start: ; don't remove the labels, they're needed to compute sizes and jumps
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
gdt_code:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

; --- User code segment (DPL=3) ---
gdt_user_code:
    dw 0xffff        ; segment limit low
    dw 0x0           ; base low
    db 0x0           ; base mid
    db 10011010b     ; flags: present=1, DPL=3, code segment, executable=1, readable=1
    db 11001111b     ; flags: granularity=1, 32-bit=1, limit high bits
    db 0x0           ; base high

; --- User data segment (DPL=3) ---
gdt_user_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b     ; flags: present=1, DPL=3, data segment, writable=1
    db 11001111b
    db 0x0


gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)

; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
USER_CODE_SEG equ gdt_user_code - gdt_start
USER_DATA_SEG equ gdt_user_data - gdt_start
