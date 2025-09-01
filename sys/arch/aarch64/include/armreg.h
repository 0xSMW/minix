/* Minimal AArch64 armreg.h for userland build, extended with TCR/TTBR bits. */
#ifndef _AARCH64_ARMREG_H_
#define _AARCH64_ARMREG_H_

#include <sys/types.h>
#include <sys/cdefs.h>

/* --- Floating-point control/status (minimal) --- */
static inline uint64_t reg_fpcr_read(void)
{
    return 0; /* default: round to nearest */
}

#ifndef FPCR_RMODE
#define FPCR_RMODE __BITS(23,22)
#endif

/* --- TCR_EL1 fields (subset used by libkvm and MD code) --- */
/* __BITS provided by <sys/cdefs.h> */

/* Granule size for TTBR1 region */
#define TCR_TG1         __BITS(31,30)
/* Encodings for TG1 (note: TG1 encoding differs from TG0) */
#define TCR_TG_16KB     1U     /* 01 */
#define TCR_TG_4KB      2U     /* 10 */
#define TCR_TG_64KB     3U     /* 11 */
/* Helper to place TG1 encoding */
#define TCR_TG1_VAL(v)  ((uint64_t)(v) << 30)

/* TTBR1 input address size (VA bits = 64 - T1SZ) */
#define TCR_T1SZ        __BITS(21,16)
/* Compose field values */
#define TCR_T1SZ_VAL(sz) (((uint64_t)(sz) & 0x3f) << 16)

/* Inner/Outer cacheability and shareability for TTBR1 walks */
#define TCR_IRGN1       __BITS(9,8)
#define  TCR_IRGN_NC    (0U << 8)
#define  TCR_IRGN_WBWA  (1U << 8)
#define  TCR_IRGN_WT    (2U << 8)
#define  TCR_IRGN_WBNWA (3U << 8)
#define TCR_ORGN1       __BITS(11,10)
#define  TCR_ORGN_NC    (0U << 10)
#define  TCR_ORGN_WBWA  (1U << 10)
#define  TCR_ORGN_WT    (2U << 10)
#define  TCR_ORGN_WBNWA (3U << 10)
#define TCR_SH1         __BITS(29,28)
#define  TCR_SH_NS      (0U << 28)
#define  TCR_SH_OS      (2U << 28)
#define  TCR_SH_IS      (3U << 28)

/* Other useful fields */
#define TCR_A1          __BIT(22)   /* ASID in TTBR1 if set */
#define TCR_EPD1        __BIT(23)   /* Walk disable for TTBR1 */

/* Intermediate Physical Address Size (IPS) */
#define TCR_IPS         __BITS(34,32)
#define  TCR_IPS_32BIT  (0U << 32)
#define  TCR_IPS_36BIT  (1U << 32)
#define  TCR_IPS_40BIT  (2U << 32)
#define  TCR_IPS_42BIT  (3U << 32)
#define  TCR_IPS_44BIT  (4U << 32)
#define  TCR_IPS_48BIT  (5U << 32)
#define  TCR_IPS_52BIT  (6U << 32)

/* Recommended kernel TCR(EL1) configuration for 4KB granule, 48-bit VA */
#define AARCH64_VA_BITS           48
#define AARCH64_TCR_T1SZ          (64 - AARCH64_VA_BITS)
#define AARCH64_TCR_KERNEL \
    ( TCR_TG1_VAL(TCR_TG_4KB) \
    | TCR_SH_IS \
    | TCR_ORGN_WBWA \
    | TCR_IRGN_WBWA \
    | TCR_T1SZ_VAL(AARCH64_TCR_T1SZ) \
    | TCR_IPS_48BIT )

/* TTBR base address/asid fields */
#define TTBR_ASID       __BITS(63,48)
#define TTBR_BADDR      __BITS(47,0)

/* --- System ID/feature register fields (subset) --- */
/* ID_AA64MMFR0_EL1 fields */
#define  ID_AA64MMFR0_PARange    __BITS(3,0)
#define  ID_AA64MMFR0_ASIDBits   __BITS(7,4)
#define  ID_AA64MMFR0_BigEnd     __BITS(11,8)
#define  ID_AA64MMFR0_SNSMem     __BITS(15,12)
#define  ID_AA64MMFR0_BigEndEL0  __BITS(19,16)
#define  ID_AA64MMFR0_TGran16    __BITS(23,20)
#define  ID_AA64MMFR0_TGran64    __BITS(27,24)
#define  ID_AA64MMFR0_TGran4     __BITS(31,28)

/* ID_AA64PFR0_EL1 fields (presence of FP/SIMD, ELs) */
#define  ID_AA64PFR0_EL0         __BITS(3,0)
#define  ID_AA64PFR0_EL1         __BITS(7,4)
#define  ID_AA64PFR0_EL2         __BITS(11,8)
#define  ID_AA64PFR0_EL3         __BITS(15,12)
#define  ID_AA64PFR0_FP          __BITS(19,16)
#define  ID_AA64PFR0_AdvSIMD     __BITS(23,20)

/* --- Sysreg accessor prototypes (implemented in MD code) --- */
#ifdef _KERNEL
uint64_t aarch64_reg_read_tcr_el1(void);
void     aarch64_reg_write_tcr_el1(uint64_t);
uint64_t aarch64_reg_read_ttbr1_el1(void);
void     aarch64_reg_write_ttbr1_el1(uint64_t);
uint64_t aarch64_reg_read_mair_el1(void);
void     aarch64_reg_write_mair_el1(uint64_t);
uint64_t aarch64_reg_read_midr_el1(void);
#endif

#endif /* _AARCH64_ARMREG_H_ */
