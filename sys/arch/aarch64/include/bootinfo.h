/*
 * AArch64 boot info passed from loader/firmware.
 */
#ifndef _AARCH64_BOOTINFO_H_
#define _AARCH64_BOOTINFO_H_

#include <sys/stdint.h>

typedef struct aarch64_bootinfo {
    uint64_t bi_fdt;        /* PA of flattened device tree, 0 if none */
    uint64_t bi_initrd_start; /* PA of initrd start, 0 if none */
    uint64_t bi_initrd_end;   /* PA of initrd end */
    uint64_t bi_memmap;     /* PA of memory map entries (platform-specific) */
    uint32_t bi_memmap_entries;
    uint32_t bi_reserved;
} aarch64_bootinfo_t;

#endif /* _AARCH64_BOOTINFO_H_ */
