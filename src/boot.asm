org 0x7C00
[bits 16]

PML4_ADDRESS equ 0x1000 ; there will only be 1
PDP_ADDRESS equ 0x2000 ; there will only be 1
FIRST_PD_ADDRESS equ 0x3000 ; covers the first 1 GB

KERNEL_LOCATION equ 0x100000

BootDisk: dd 0

main:

    mov [BootDisk], dl

; *****************************
; *** Load Kernel to Memory ***
; *****************************




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
    .loop:

        mov eax, 0x200000
        mul ecx
        or eax, 0x83 ; 
        mov [FIRST_PD_ADDRESS + ecx * 8], eax
        
        inc ecx
        cmp ecx, 512
        jne .loop

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



; *************************
; *** Simple GDT Struct ***
; *************************
GDT:
    dq 0
.Code: equ $ - GDT
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.Pointer:
    dw $ - GDT - 1
    dq GDT


; ***********************
; *** Long Mode Start ***
; ***********************
[BITS 64]
[extern kernelMain]
 
LongModeStart:
    
    ; clear registers
    cli
    mov ax, 0;GDT.Data
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

    mov dword[0xb8000], 0x2f4b2f4f

    jmp $

times 510-($-$$) db 0
db 0x55, 0xaa