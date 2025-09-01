#include "kernel/kernel.h"
#include "kernel/proc.h"

extern char aarch64_vectors[];

static inline void write_vbar_el1(void *base)
{
    __asm__ volatile(
        "msr VBAR_EL1, %0\n\t"
        "isb\n\t"
        : : "r"(base) : "memory");
}

void exception_init(void)
{
    write_vbar_el1(aarch64_vectors);
}
