org 0x7C00
[bits 16]

; paging addresses
PML4_ADDRESS equ 0x1000 ; there will only be 1
PDP_ADDRESS equ 0x2000 ; there will only be 1
FIRST_PD_ADDRESS equ 0x3000 ; covers the first 1 GB

; idt addresses
IDT_ADDRESS equ 0x4000

; memory map addresses
MEMMAP_ADDRESS equ 0x7000

; kernel addresses
STACK_BASE_ADDRESS equ 0x9FBFF ; highest memory in kernel
KERNEL_ADDRESS equ 0x7E00

BootDisk: dd 0

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

    mov ah, 0x02 ; command
    mov bx, KERNEL_ADDRESS
    mov al, 0x20 ; amount to read
    mov ch, 0x00 ; cylinder
    mov dh, 0x00 ; head
    mov cl, 0x02 ; sector
    mov dl, [BootDisk]

    int 0x13

    mov bl, 'K'
    jc Error


; *****************
; *** Setup IDT ***
; *****************

    ; todo


; **************************
; *** Collect Memory Map ***
; **************************
    
    mov ecx, MEMMAP_ADDRESS ; next address
    xor ebx, ebx
    mov es, ebx
    .getNextMemMap:

        ; E820 bios call 

        mov di, cx ; address
        push ecx

        mov eax, 0xE820 ; command
        mov ecx, 24 ; get all 24 bytes
        mov edx, 0x0534D4150
        int 0x15

        pop ecx

        ; check for error
        push ebx
        mov bl, 'M'
        jc Error
        pop ebx

        ; loop
        add ecx, 24
        cmp ebx, 0
        jne .getNextMemMap
        
    ; if ecx < 128 enteries
    cmp ecx, 0x7C00
    je .skipMemMapEnd

    ; skip 8 byte base and 8 byte length
    add ecx, 16 

    ; fill type field with 0xFFFFFFFF to mark end
    mov eax, 0xFFFFFFFF
    mov [ecx], eax

    .skipMemMapEnd:



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
.Pointer:
    dw $ - GDT - 1
    dq GDT


; **********************
; *** Error Handling ***
; **********************

; param: bl = error code(ASCII)
Error:
    mov ah, 0x0e
    
    mov al, 'E'
    int 0x10

    mov al, 'R'
    int 0x10

    mov al, 'R'
    int 0x10

    mov al, ':'
    int 0x10

    mov al, bl
    int 0x10

    jmp $


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

    ; clear to blue screen
    mov edi, 0xB8000
    mov rax, 0x1F201F201F201F20
    mov ecx, 500
    rep stosq

    ; jump to kernel
    jmp KERNEL_ADDRESS
    jmp $

times 510-($-$$) db 0
db 0x55, 0xaa