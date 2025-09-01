/*
 * AArch64 kcore.h
 * Minimal cpu_kcore_hdr_t definition to satisfy libkvm.
 */
#ifndef _AARCH64_KCORE_H_
#define _AARCH64_KCORE_H_

#include <sys/stdint.h>
#include <sys/kcore.h>

typedef struct cpu_kcore_hdr {
    uint64_t    kh_tcr1;    /* Captured TCR_EL1 value */
    uint64_t    kh_ttbr1;   /* TTBR1_EL1 (ASID in [63:48], base in [47:0]) */
    /*
     * RAM segments: terminate with size == 0
     * The actual dump layout may place more entries than 1.
     */
    phys_ram_seg_t kh_ramsegs[1];
} cpu_kcore_hdr_t;

#endif /* _AARCH64_KCORE_H_ */

