org 0x7C00
[bits 16]

KERNEL_LOCATION equ 0x1000
BootDisk: dd 0

MemMapBuffer: times 24 db 0 ; create a 24 byte space for data
MemMapCount: dd 0

main:

    ; collect data
    mov [BootDisk], dl ; save current disk number

    ; read kernel from disk
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov bp, 0x8000
    mov sp, bp

    mov ebx, KERNEL_LOCATION
    mov dh, 0x20 ; how much to read

    mov ah, 0x02 ; sector
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [BootDisk]

    int 0x13

    ; enable a20 line
    ; is it working???
    ; what does it do???
    in al, 0x92
    or al, 2
    out 0x92, al

    ; clear screen
    mov ah, 0x0
    mov al, 0x3
    int 0x10

    ; collect memory map
    xor ebx, ebx
    .loop:
        ; E820 BIOS call 
        
        ; es * 16 + di = MemMapBuffer
        ; es = MemMapBuffer / 16
        ; di = MemMapBuffer - (es * 16)
        mov dx, 0
        mov ax, MemMapBuffer
        mov cx, 16
        div cx

        mov es, ax
        mov di, dx

        mov eax, 0xE820
        mov edx, 0x0534D4150
        mov ecx, 24 ; get all 24 bytes

        int 0x15 ; call
        
        ;check for error
        jc .fail
        jmp .success

        .fail:
            mov ah, 0x0e
            mov al, 'F'
            int 0x10
            jmp $

        .success:

        ; pass data to stack
        mov eax, [MemMapBuffer + 20] ; ACPI
        push eax

        mov eax, [MemMapBuffer + 16] ; Type
        push eax

        mov eax, [MemMapBuffer + 12] ; Length High
        push eax

        mov eax, [MemMapBuffer + 8] ; Length Low
        push eax

        mov eax, [MemMapBuffer + 4] ; Base High
        push eax

        mov eax, [MemMapBuffer] ; Base Low
        push eax

        ; increment count
        mov eax, [MemMapCount]
        add eax, 1
        mov [MemMapCount], eax

        ; end loop
        cmp ebx, 0
        jne .loop

        ; push pointer
        push esp

        ; push count
        mov eax, [MemMapCount]
        push eax
        
    .loop_end:

    ; enter protected mode
    jmp LoadProtectedMode

; =======================
; === Memory Map Data ===
; =======================

; todo

; ===============================
; === Global Descriptor Table ===
; ===============================
LoadProtectedMode:

    cli
    lgdt[GDT_Descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:ProtectedModeStart

GDT_Start:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0x9A ; 10011010; pres, priv, type, type flags
        db 0xCF ; 11001111; other flags
        db 0
    data_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0x92 ; 10010010; pres, priv, type, type flags
        db 0xCF ; 11001111; other flags
        db 0
GDT_End:

GDT_Descriptor:
    dw GDT_End - GDT_Start - 1
    dd GDT_Start
    CODE_SEG equ code_descriptor - GDT_Start
    DATA_SEG equ data_descriptor - GDT_Start


; ============================
; === Entering The Kernel ===
; ============================
[bits 32]
ProtectedModeStart:

    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; why???
    ;mov ebp, 0x90000
    ;mov esp, ebp

    jmp KERNEL_LOCATION

times 510-($-$$) db 0
db 0x55, 0xaa