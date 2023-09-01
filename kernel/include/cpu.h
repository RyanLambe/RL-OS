#pragma once

// functions from: https://wiki.osdev.org/Inline_Assembly/Examples

static inline void outb(unsigned short port, unsigned char val){
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) :"memory");
}

static inline unsigned char inb(unsigned short port)
{
    unsigned char ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void io_wait()
{
    outb(0x80, 0);
}