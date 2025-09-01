#ifndef _AARCH64_STACKFRAME_H
#define _AARCH64_STACKFRAME_H

#include <sys/types.h>

typedef u64_t reg_t;         /* machine register */

/*
 * AArch64 general-purpose registers:
 *  x0..x30, with x29=fp, x30=lr, separate sp, pc not directly visible.
 * Keep retreg/xN naming similar to other arches for MINIX compatibility.
 */
struct stackframe_s {
    reg_t retreg;  /* x0 */
    reg_t r1;      /* x1 */
    reg_t r2;      /* x2 */
    reg_t r3;      /* x3 */
    reg_t r4;      /* x4 */
    reg_t r5;      /* x5 */
    reg_t r6;      /* x6 */
    reg_t r7;      /* x7 */
    reg_t r8;      /* x8 */
    reg_t r9;      /* x9 */
    reg_t r10;     /* x10 */
    reg_t r11;     /* x11 */
    reg_t r12;     /* x12 */
    reg_t r13;     /* x13 */
    reg_t r14;     /* x14 */
    reg_t r15;     /* x15 */
    reg_t r16;     /* x16 */
    reg_t r17;     /* x17 */
    reg_t r18;     /* x18 (platform) */
    reg_t r19;     /* x19 */
    reg_t r20;     /* x20 */
    reg_t r21;     /* x21 */
    reg_t r22;     /* x22 */
    reg_t r23;     /* x23 */
    reg_t r24;     /* x24 */
    reg_t r25;     /* x25 */
    reg_t r26;     /* x26 */
    reg_t r27;     /* x27 */
    reg_t r28;     /* x28 */
    reg_t fp;      /* x29 */
    reg_t lr;      /* x30 */
    reg_t sp;      /* stack pointer */
    reg_t pc;      /* program counter */
    reg_t psr;     /* PSTATE/SPSR */
};

#endif /* _AARCH64_STACKFRAME_H */

