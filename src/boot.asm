org 0x7C00
[bits 16]

PML4_ADDRESS equ 0x2000 ; there will only be 1
PDP_ADDRESS equ 0x3000 ; there will only be 1
FIRST_PD_ADDRESS equ 0x4000 ; covers the first 1 GB

STACK_BASE_ADDRESS equ 0x8000
KERNEL_ADDRESS equ 0x1000 ; note: address hardcoded when loaded

BootDisk: dd 0

MemMapBuffer: times 24 db 0 ; create a 24 byte space for data
MemMapCount: dd 0

_start:

    ; save bootdisk number
    mov [BootDisk], dl

    ; clear registers
    xor ax, ax
    mov es, ax
    mov ds, ax

    ; setup stack
    mov ebp, STACK_BASE_ADDRESS
    mov esp, ebp
    
    ; enable A20 line to access the first 16mb of memory instead of 1mb
    in al, 0x92
    or al, 2
    out 0x92, al

    ; clear screen
    mov ah, 0x0
    mov al, 0x3
    int 0x10


; *****************************
; *** Load Kernel to Memory ***
; *****************************

    mov bx, KERNEL_ADDRESS
    mov dh, 0x20 ; how much to read

    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [BootDisk]

    int 0x13


; **************************
; *** Collect Memory Map ***
; **************************

    ; put in specific location(rather then the stack)


; **********************
; *** Load Long Mode ***
; **********************

    ; put pdp address in pml4
    mov eax, PDP_ADDRESS
    or eax, 0x3
    mov [PML4_ADDRESS], eax

    ; put the first pd address in pdp
    mov eax, FIRST_PD_ADDRESS
    or eax, 0x3
    mov [PDP_ADDRESS], eax


    ; fill in PD with 2mb pages
    mov ecx, 0 ; counter
    .FillPD:

        mov eax, 0x200000
        mul ecx
        or eax, 0x83 ; 
        mov [FIRST_PD_ADDRESS + ecx * 8], eax
        
        inc ecx
        cmp ecx, 512
        jne .FillPD

    ; pass pml4 to cpu
    mov eax, PML4_ADDRESS
    mov cr3, eax

    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; enable long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging & protected mode
    mov eax, cr0
    or eax, 1 << 31 | 1 << 0
    mov cr0, eax

    ; jump to long mode
    lgdt [GDT.Pointer]
    jmp GDT.Code:LongModeStart


; *******************************
; *** Global Descriptor Table ***
; *******************************
GDT:
.Null:
    dq 0
.Code: equ $ - GDT
    dd 0xFFFF
    db 0
    db 0x9A ; 10011010
    db 0xAF ; 10101111
    db 0
.Data: equ $ - GDT
    dd 0xFFFF
    db 0
    db 0x92 ; 10010010
    db 0xCF ; 11001111
    db 0
.TSS: equ $ - GDT
    dd 0x00000068
    dd 0x00CF8900
.Pointer:
    dw $ - GDT - 1
    dq GDT


; ***********************
; *** Long Mode Start ***
; ***********************
[BITS 64]
 
LongModeStart:
    
    ; clear registers
    cli
    mov ax, GDT.Data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; clear to blue screen???
    mov edi, 0xB8000
    mov rax, 0x1F201F201F201F20
    mov ecx, 500
    rep stosq

    jmp KERNEL_ADDRESS ; todo move Kernel to better address
    jmp $

times 510-($-$$) db 0
db 0x55, 0xaa