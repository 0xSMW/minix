#include <machine/asm.h>

.text
.align 11
.global aarch64_vectors
aarch64_vectors:
    // EL1t sync
    b .
    // EL1t irq
    b .
    // EL1t fiq
    b .
    // EL1t serr
    b .
    // EL1h sync
    b .
    // EL1h irq
    bl el1h_irq_handler
    eret
    // EL1h fiq
    b .
    // EL1h serr
    b .
    // EL0 sync (SVC from user)
    b .
    // EL0 irq
    bl el1h_irq_handler
    eret
    // EL0 fiq
    b .
    // EL0 serr
    b .
    // EL0 32bit sync
    b .
    // EL0 32bit irq
    bl el1h_irq_handler
    eret
    // EL0 32bit fiq
    b .
    // EL0 32bit serr
    b .

.global el1h_irq_handler
el1h_irq_handler:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    bl _C_LABEL(context_stop_idle)
    bl _C_LABEL(gic_read_irq)
    mov x19, x0            // save irq number
    bl _C_LABEL(irq_handle)
    mov x0, x19
    bl _C_LABEL(gic_eoi)
    ldp x29, x30, [sp], #16
    ret
