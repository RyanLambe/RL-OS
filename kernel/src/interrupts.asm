[bits 64]
[extern ISRHandler]
[extern IRQHandler]

%macro pusha 0
	push rax
	push rcx
	push rdx
	push rbx
	push rsp
	push rbp
	push rsi
	push rdi
%endmacro

%macro popa 0
	pop rdi
	pop rsi
	pop rbp
	pop rsp
	pop rbx
	pop rcx
	pop rdx
	pop rax
%endmacro

%macro ISR 1
  global isr%1
  isr%1:
    push 0
    push %1
    jmp ISRGeneral
%endmacro

%macro ISR_ERR 1
  global isr%1
  isr%1:
    push %1
    jmp ISRGeneral
%endmacro

%macro IRQ 2
  global irq%1
  irq%1:
    push 0
    push %2
    jmp IRQGeneral
%endmacro


ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_ERR 8
ISR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47


ISRGeneral:
	pusha

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

	popa

    ; return
    add rsp, 16
    iretq


IRQGeneral:
	pusha

    ; save original segment
    mov ax, ds
    push rax

    ; load the kernel data segment descriptor
    mov ax, 0x20
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; call irq handler
    call IRQHandler

    ; reload the original segment
    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa

    ; return
    add rsp, 16
    iretq