#ifndef _AARCH64_ACONST_H
#define _AARCH64_ACONST_H

/* Program status defaults. Keep minimal; detailed PSTATE bits set in asm. */
#define INIT_PSR      (0)
#define INIT_TASK_PSR (0)

/* Size reserved at top of kernel stack for global info (mirror ARM). */
#define AARCH64_STACK_TOP_RESERVED (2 * sizeof(reg_t))

#endif /* _AARCH64_ACONST_H */

