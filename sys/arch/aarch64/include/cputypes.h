/*
 * AArch64 CPU type and feature flags (compile-time constants only).
 */
#ifndef _AARCH64_CPUTYPES_H_
#define _AARCH64_CPUTYPES_H_

/* Architecture level (ARMv8.x) */
#define AARCH64_ARCH_V8      0x08
#define AARCH64_ARCH_V8_1    0x09
#define AARCH64_ARCH_V8_2    0x0A
#define AARCH64_ARCH_V8_3    0x0B
#define AARCH64_ARCH_V8_4    0x0C
#define AARCH64_ARCH_V8_5    0x0D

/* Feature flags (subset) */
#define AARCH64_FEAT_ATOMICS   __BIT(0)  /* LSE atomics */
#define AARCH64_FEAT_FP        __BIT(1)  /* FP supported */
#define AARCH64_FEAT_ASIMD     __BIT(2)  /* Advanced SIMD */
#define AARCH64_FEAT_CRC32     __BIT(3)  /* CRC32 */
#define AARCH64_FEAT_AES       __BIT(4)
#define AARCH64_FEAT_PMULL     __BIT(5)
#define AARCH64_FEAT_SHA1      __BIT(6)
#define AARCH64_FEAT_SHA2      __BIT(7)
#define AARCH64_FEAT_SHA3      __BIT(8)
#define AARCH64_FEAT_SM3       __BIT(9)
#define AARCH64_FEAT_SM4       __BIT(10)

#ifndef __BIT
#define __BIT(__n) (1ULL << (__n))
#endif

#endif /* _AARCH64_CPUTYPES_H_ */
