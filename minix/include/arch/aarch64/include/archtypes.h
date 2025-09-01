#ifndef _AARCH64_TYPES_H
#define _AARCH64_TYPES_H

#include <minix/sys_config.h>
#include <machine/stackframe.h>
#include <sys/cdefs.h>

typedef struct segframe {
	reg_t	p_ttbr;		/* page table root (TTBR) */
	u64_t	*p_ttbr_v;
	char	*fpu_state;
} segframe_t;

struct cpu_info {
	u32_t	arch;
	u32_t	implementer;
	u32_t	part;
	u32_t	variant;
	u32_t	freq;      /* in MHz */
	u32_t	revision;
};

typedef u64_t atomic_t; /* aligned 64-bit access is atomic on AArch64 */

#endif /* _AARCH64_TYPES_H */

