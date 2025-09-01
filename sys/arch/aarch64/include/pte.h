/*
 * AArch64 pte.h
 * Minimal AArch64 PTE/descriptor definitions for userland tools (libkvm).
 * This is not a full kernel pmap; only the pieces required by consumers.
 */
#ifndef _AARCH64_PTE_H_
#define _AARCH64_PTE_H_

#include <sys/types.h>
#include <sys/stdint.h>
#include <sys/cdefs.h>

/* Descriptor and table entry types */
typedef uint64_t pt_entry_t;   /* L3: page entries */
typedef uint64_t pd_entry_t;   /* L0-L2: table/block entries */

/* Common descriptor bits */
/* __BIT/__BITS provided by <sys/cdefs.h> */

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

/*
 * Page/block address field helpers per level with 4KB granule
 * (48-bit VA configuration typical for EL1/EL0).
 *
 * L0: [47:39] index (table only for 4K TG1)
 * L1: [47:30] block (1GiB) or table next level
 * L2: [47:21] block (2MiB) or table next level
 * L3: [47:12] page (4KiB)
 */
#define LX_IDXBITS      9ULL
#define LX_IDXMASK      ((1ULL << LX_IDXBITS) - 1)
#define L0_SHIFT        39
#define L1_SHIFT        30
#define L2_SHIFT        21
#define L3_SHIFT        12

#define L0_IDX(va)      (((uint64_t)(va) >> L0_SHIFT) & LX_IDXMASK)
#define L1_IDX(va)      (((uint64_t)(va) >> L1_SHIFT) & LX_IDXMASK)
#define L2_IDX(va)      (((uint64_t)(va) >> L2_SHIFT) & LX_IDXMASK)
#define L3_IDX(va)      (((uint64_t)(va) >> L3_SHIFT) & LX_IDXMASK)

#define PTE_ADDR_Lx(pte, shift)   ((pte) & __BITS(47, (shift)))
#define PTE_ADDR_L1(pde)          PTE_ADDR_Lx((pde), L1_SHIFT)
#define PTE_ADDR_L2(pde)          PTE_ADDR_Lx((pde), L2_SHIFT)
#define PTE_ADDR_L3(pte)          PTE_ADDR_Lx((pte), L3_SHIFT)

/* Block/Page sizes with 4KB granule. */
#define AARCH64_BLOCK_L1_SIZE    (1ULL << L1_SHIFT)   /* 1 GiB */
#define AARCH64_BLOCK_L2_SIZE    (1ULL << L2_SHIFT)   /* 2 MiB */
#define AARCH64_PAGE_L3_SIZE     (1ULL << L3_SHIFT)   /* 4 KiB */

/* Common MAIR attribute index selections (indices used in PTE_ATTRINDX) */
#define MAIR_IDX_DEVICE_nGnRnE   0U  /* Strongly-ordered device */
#define MAIR_IDX_NORMAL_NC       1U  /* Normal Non-Cacheable */
#define MAIR_IDX_NORMAL_WB       2U  /* Normal Write-Back Cacheable */

/*
 * Supported granules: 4KB, 16KB, 64KB.
 * This header assumes 4KB granule for index/size helpers above, which
 * matches the most common configuration on Apple Silicon VMs. Tools that
 * consume these should consult TCR_EL1.TG1 and adjust if needed.
 */

/*
 * TTBR base address mask (excludes ASID in [63:48]).
 * The libkvm code further clears low alignment bits as needed.
 */
#ifndef TTBR_BADDR
#define TTBR_BADDR      __BITS(47,0)
#endif

#endif /* _AARCH64_PTE_H_ */
