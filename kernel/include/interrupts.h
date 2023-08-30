#pragma once

struct __attribute__((packed)) IDTEntry {

    unsigned short pointerLow;
    unsigned short selector;
    unsigned char  ist;
    unsigned char  flags;
    unsigned short pointerMid;
    unsigned int pointerHigh;
    unsigned int zero = 0;
};

enum GDTSegment{
    Code = 0x10,
    Data = 0x20,
    UserCode = 0x30,
    UserData = 0x40,
};

enum PrivilegeLevel{
    Kernel = 0b00,
    User = 0b11,
};

struct Registers
{
    unsigned long ds;                  // Data segment selector
    //unsigned long rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax; // registers
    unsigned long interrupt, errorCode;    // Interrupt number and error code
    unsigned long eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
};

void SetupIDT();

// is there a better way :(
extern "C" void isr0 ();
extern "C" void isr1 ();
extern "C" void isr2 ();
extern "C" void isr3 ();
extern "C" void isr4 ();
extern "C" void isr5 ();
extern "C" void isr6 ();
extern "C" void isr7 ();
extern "C" void isr8 ();
extern "C" void isr9 ();
extern "C" void isr10 ();
extern "C" void isr11 ();
extern "C" void isr12 ();
extern "C" void isr13 ();
extern "C" void isr14 ();
extern "C" void isr15 ();
extern "C" void isr16 ();
extern "C" void isr17 ();
extern "C" void isr18 ();
extern "C" void isr19 ();
extern "C" void isr20 ();
extern "C" void isr21 ();
extern "C" void isr22 ();
extern "C" void isr23 ();
extern "C" void isr24 ();
extern "C" void isr25 ();
extern "C" void isr26 ();
extern "C" void isr27 ();
extern "C" void isr28 ();
extern "C" void isr29 ();
extern "C" void isr30 ();
extern "C" void isr31 ();
