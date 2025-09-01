/*
 * AArch64 FP/SIMD state description (headers only).
 */
#ifndef _AARCH64_FPU_H_
#define _AARCH64_FPU_H_

#include <sys/stdint.h>

/* AArch64 has 32 128-bit V registers shared between FP/SIMD. */
typedef struct {
    uint8_t vreg[32][16]; /* V0..V31 */
    uint32_t fpsr;        /* Floating-Point Status Register */
    uint32_t fpcr;        /* Floating-Point Control Register */
} aarch64_fpsimd_state_t;

#ifdef _KERNEL
struct lwp;
struct cpu_info;

void    fpuinit(struct cpu_info *);
void    fpusave_lwp(struct lwp *, bool);
void    fpusave_cpu(bool);

void    process_read_fpregs(struct lwp *, aarch64_fpsimd_state_t *);
void    process_write_fpregs(struct lwp *, const aarch64_fpsimd_state_t *);
#endif

#endif /* _AARCH64_FPU_H_ */
