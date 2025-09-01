#include <machine/asm.h>

.text

/* Enable/disable IRQs: DAIF I bit */
ENTRY(interrupts_enable)
    msr daifclr, #2
    ret

ENTRY(interrupts_disable)
    msr daifset, #2
    ret

/* int copy_msg_from_user(message *user, message *dst) */
ENTRY(copy_msg_from_user)
    // x0 = user src, x1 = dst
    stp x2, x3, [sp, #-16]!
    // copy 64 bytes in 8 x 8-byte pairs
    ldp x2, x3, [x0]
    stp x2, x3, [x1]
    ldp x2, x3, [x0, #16]
    stp x2, x3, [x1, #16]
    ldp x2, x3, [x0, #32]
    stp x2, x3, [x1, #32]
    ldp x2, x3, [x0, #48]
    stp x2, x3, [x1, #48]
    mov x0, #0
    ldp x2, x3, [sp], #16
    ret

/* int copy_msg_to_user(message *src, message *user) */
ENTRY(copy_msg_to_user)
    // x0 = src, x1 = user dst
    stp x2, x3, [sp, #-16]!
    ldp x2, x3, [x0]
    stp x2, x3, [x1]
    ldp x2, x3, [x0, #16]
    stp x2, x3, [x1, #16]
    ldp x2, x3, [x0, #32]
    stp x2, x3, [x1, #32]
    ldp x2, x3, [x0, #48]
    stp x2, x3, [x1, #48]
    mov x0, #0
    ldp x2, x3, [sp], #16
    ret

/* Pointer fault continuation */
ENTRY(__user_copy_msg_pointer_failure)
    mov x0, #-1
    ret

/*
 * void restore_user_context(struct proc *p)
 * Load user registers from p->p_reg and return to EL0.
 * Layout of p->p_reg matches minix/include/arch/aarch64/include/stackframe.h
 */
ENTRY(restore_user_context)
    // x0 = struct proc *p
    mov     x9, x0                 // keep base pointer to proc
    // p_reg is first field in struct proc, so base = p
    mov     x10, x9

    // Set SP_EL0, ELR_EL1, SPSR_EL1 from saved frame
    ldr     x11, [x10, #248]       // sp
    msr     SP_EL0, x11
    ldr     x11, [x10, #256]       // pc
    msr     ELR_EL1, x11
    ldr     x11, [x10, #264]       // psr
    msr     SPSR_EL1, x11

    // Restore callee-saved and general registers.
    // Be careful to restore x1/x0 last as we need x10 as base.

    // x2..x7
    ldr     x2,  [x10, #16]
    ldr     x3,  [x10, #24]
    ldr     x4,  [x10, #32]
    ldr     x5,  [x10, #40]
    ldr     x6,  [x10, #48]
    ldr     x7,  [x10, #56]
    // x8..x15
    ldr     x8,  [x10, #64]
    ldr     x12, [x10, #96]
    ldr     x13, [x10, #104]
    ldr     x14, [x10, #112]
    ldr     x15, [x10, #120]
    // x9..x11 (x10 used as base, restore x9 later)
    ldr     x11, [x10, #88]
    ldr     x9,  [x10, #72]        // overwrite base copy now; base no longer needed
    // x16..x18
    ldr     x16, [x10, #128]
    ldr     x17, [x10, #136]
    ldr     x18, [x10, #144]
    // x19..x28
    ldr     x19, [x10, #152]
    ldr     x20, [x10, #160]
    ldr     x21, [x10, #168]
    ldr     x22, [x10, #176]
    ldr     x23, [x10, #184]
    ldr     x24, [x10, #192]
    ldr     x25, [x10, #200]
    ldr     x26, [x10, #208]
    ldr     x27, [x10, #216]
    ldr     x28, [x10, #224]
    // fp, lr
    ldr     x29, [x10, #232]
    ldr     x30, [x10, #240]
    // finally x1 and x0
    ldr     x1,  [x10, #8]
    ldr     x0,  [x10, #0]

    // Return to EL0 with restored state
    eret
