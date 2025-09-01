/* Minimal AArch64 armreg.h for userland build, extended with TCR/TTBR bits. */
#ifndef _AARCH64_ARMREG_H_
#define _AARCH64_ARMREG_H_

#include <sys/types.h>

/* --- Floating-point control/status (minimal) --- */
static inline uint64_t reg_fpcr_read(void)
{
    return 0; /* default: round to nearest */
}

#ifndef FPCR_RMODE
#define FPCR_RMODE __BITS(23,22)
#endif

/* --- TCR_EL1 fields (subset used by libkvm) --- */
#ifndef __BITS
#define __BITS(__m, __n) (((~0ULL) - ((1ULL << (__n)) - 1)) & (((1ULL << (__m)) - 1) | (1ULL << (__m))))
#endif

/* Granule size for TTBR1 region */
#define TCR_TG1         __BITS(31,30)
/* Encodings for TG1 (note: TG1 encoding differs from TG0) */
#define TCR_TG_16KB     1U     /* 01 */
#define TCR_TG_4KB      2U     /* 10 */
#define TCR_TG_64KB     3U     /* 11 */

/* TTBR1 input address size (VA bits = T1SZ + base per TG1) */
#define TCR_T1SZ        __BITS(21,16)

/* TTBR base address/asid fields */
#define TTBR_ASID       __BITS(63,48)
#define TTBR_BADDR      __BITS(47,0)

#endif /* _AARCH64_ARMREG_H_ */
