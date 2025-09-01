#ifndef __SMP_AARCH64_H__
#define __SMP_AARCH64_H__

#include "arch_proto.h" /* K_STACK_SIZE */

#ifndef __ASSEMBLY__

/* returns the current cpu id; UP fallback returns 0 */
#define cpuid	(0)

/* single-CPU fallback: finish boot on BSP only */
#define smp_single_cpu_fallback() do { \
    bsp_cpu_id = 0; \
    ncpus = 1; \
    bsp_finish_booting(); \
} while(0)

#define barrier() __asm__ volatile("dmb ish" ::: "memory")

#endif /* __ASSEMBLY__ */

#endif /* __SMP_AARCH64_H__ */

