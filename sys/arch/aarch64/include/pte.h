/*
 * AArch64 pte.h
 * Minimal AArch64 PTE/descriptor definitions for userland tools (libkvm).
 * This is not a full kernel pmap; only the pieces required by consumers.
 */
#ifndef _AARCH64_PTE_H_
#define _AARCH64_PTE_H_

#include <sys/types.h>
#include <sys/stdint.h>

/* Descriptor types and validity */
typedef uint64_t pt_entry_t;
typedef uint64_t pd_entry_t;

/* Common descriptor bits */
#ifndef __BIT
#define __BIT(__n) (1ULL << (__n))
#endif
#ifndef __BITS
#define __BITS(__m, __n) (((~0ULL) - ((1ULL << (__n)) - 1)) & (((1ULL << (__m)) - 1) | (1ULL << (__m))))
#endif

/* Level descriptor validity/type (applies to L0-L2) */
#define LX_VALID        __BIT(0)
#define LX_TYPE         __BIT(1)   /* 0 = block, 1 = table (L0-L2) */
#define LX_TYPE_BLK     0          /* for kvm’s (pte & LX_TYPE) == LX_TYPE_BLK test */
/* For L3, the same bit selects page vs reserved; not used here. */

/* AArch64 kernel segment (TTBR1) canonical address check (48-bit VA default). */
#define AARCH64_KSEG_START   0xffff000000000000ULL
#define AARCH64_KSEG_MASK    0xffff000000000000ULL

/* PTE attribute field definitions (subset) */
#define PTE_ATTRINDX     __BITS(4,2)   /* MAIR AttrIdx */
#define  PTE_ATTRINDX_SET(i)   (((uint64_t)(i) & 0x7) << 2)
#define PTE_AP           __BITS(7,6)   /* Access permissions */
#define  PTE_AP_RW_EL0   (0U << 6)
#define  PTE_AP_RW_EL1   (1U << 6)
#define  PTE_AP_RO_EL0   (2U << 6)
#define  PTE_AP_RO_EL1   (3U << 6)
#define PTE_SH           __BITS(9,8)   /* Shareability */
#define  PTE_SH_NS       (0U << 8)
#define  PTE_SH_OS       (2U << 8)
#define  PTE_SH_IS       (3U << 8)
#define PTE_AF           __BIT(10)     /* Access Flag */
#define PTE_NG           __BIT(11)     /* Not-Global */
#define PTE_PXN          __BIT(53)     /* Privileged eXecute-Never */
#define PTE_UXN          __BIT(54)     /* Unprivileged eXecute-Never */

/* Common MAIR attribute index selections (indices used in PTE_ATTRINDX) */
#define MAIR_IDX_DEVICE_nGnRnE   0U  /* Strongly-ordered device */
#define MAIR_IDX_NORMAL_NC       1U  /* Normal Non-Cacheable */
#define MAIR_IDX_NORMAL_WB       2U  /* Normal Write-Back Cacheable */

/*
 * Supported granules: 4KB, 16KB, 64KB. libkvm reads TCR_EL1.TG1 to select
 * translation parameters; helpers intentionally minimal in userland.
 */

/*
 * TTBR base address mask (excludes ASID in [63:48]).
 * The libkvm code further clears low alignment bits as needed.
 */
#ifndef TTBR_BADDR
#define TTBR_BADDR      __BITS(47,0)
#endif

#endif /* _AARCH64_PTE_H_ */
