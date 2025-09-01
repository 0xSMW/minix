#ifndef __SYS_VM_AARCH64_H__
#define __SYS_VM_AARCH64_H__

/*
 * Minimal AArch64 VM header for userland builds. Detailed MMU/VM
 * definitions are kernel/arch specific and will land with the kernel.
 */

#ifndef __ASSEMBLY__
#include <minix/type.h>

/* structure used by VM to pass data to the kernel while enabling paging */
struct vm_ep_data {
    struct mem_map *mem_map;
    vir_bytes       data_seg_limit;
};
#endif

#endif /* __SYS_VM_AARCH64_H__ */

