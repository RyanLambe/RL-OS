[bits 64]
[extern ISRHandler]


%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
  global isr%1        ; %1 accesses the first parameter.
  isr%1:
    push 0
    push %1
    jmp ISRGeneral
%endmacro

%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    push %1
    jmp ISRGeneral
%endmacro


ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31


ISRGeneral:

    ; save original segment
    mov ax, ds
    push rax

    ; load the kernel data segment descriptor
    mov ax, 0x20
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; call isr handler
    call ISRHandler

    ; reload the original segment
    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; return
    add rsp, 16
    iretq