/* Minimal interrupt distribution interfaces for AArch64 SMP. */
#ifndef _AARCH64_INTR_DISTRIBUTE_H_
#define _AARCH64_INTR_DISTRIBUTE_H_

#ifdef _KERNEL
#include <sys/kcpuset.h>

struct intrhand;
int intr_distribute(struct intrhand *, const kcpuset_t *, kcpuset_t *);
int intr_distribute_handler(const char *, const kcpuset_t *, kcpuset_t *);

#endif /* _KERNEL */

#endif /* _AARCH64_INTR_DISTRIBUTE_H_ */
