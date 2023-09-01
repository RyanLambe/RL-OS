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
    unsigned long ds;                  // data segment selector
    unsigned long rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax; // registers
    unsigned long interrupt, errorCode;    // interrupt number and error code
    unsigned long eip, cs, eflags, useresp, ss; // pushed by the processor automatically.
};

//typedef void(*InterruptHandler)(Registers);