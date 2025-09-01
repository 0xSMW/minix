/*
 * AArch64 extended CPU state container for optional features (e.g., SVE).
 */
#ifndef _AARCH64_CPU_EXTENDED_STATE_H_
#define _AARCH64_CPU_EXTENDED_STATE_H_

typedef struct {
    /* Reserved for future extensions (e.g., SVE state). */
    unsigned int size;
} aarch64_extstate_t;

#endif /* _AARCH64_CPU_EXTENDED_STATE_H_ */
