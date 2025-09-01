/*
 * AArch64 vmparam.h — 48-bit VA layout with 4KB granule by default.
 */
#ifndef _AARCH64_VMPARAM_H_
#define _AARCH64_VMPARAM_H_

#include <sys/types.h>

/* Page geometry */
#define PAGE_SHIFT   12
#define PAGE_SIZE    (1UL << PAGE_SHIFT)
#define PAGE_MASK    (PAGE_SIZE - 1)

/*
 * 48-bit VA split:
 *  - User:   0x0000000000000000 .. 0x0000ffffffffffff
 *  - Kernel: 0xffff000000000000 .. 0xffffffffffffffff
 */
#define AARCH64_USER_VA_BITS     48
#define AARCH64_KERNEL_VA_BITS   48

#define KERNEL_BASE              0xffff000000000000UL

#define VM_MIN_ADDRESS           ((vaddr_t)PAGE_SIZE)
#define VM_MAXUSER_ADDRESS       ((vaddr_t)0x0000fffffffff000UL)
#define VM_MAX_ADDRESS           VM_MAXUSER_ADDRESS

#define VM_MIN_KERNEL_ADDRESS    ((vaddr_t)KERNEL_BASE)
#define VM_MAX_KERNEL_ADDRESS    ((vaddr_t)-(PAGE_SIZE+1))

/* Stack top for user processes */
#define USRSTACK                  VM_MAXUSER_ADDRESS

/* Sizes for various kernel submaps */
#define VM_PHYS_SIZE             (300 * PAGE_SIZE)

/* Physical segment handling */
#define VM_PHYSSEG_MAX           64
#define VM_PHYSSEG_STRAT         VM_PSTRAT_BSEARCH

/* Free lists (default only for now) */
#define VM_NFREELIST             1
#define VM_FREELIST_DEFAULT      0

#endif /* _AARCH64_VMPARAM_H_ */
