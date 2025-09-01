/*
 * AArch64 PIC abstraction — primarily for GICv2/v3.
 */
#ifndef _AARCH64_PIC_H_
#define _AARCH64_PIC_H_

struct cpu_info;

struct pic {
    const char *pic_name;
    int pic_type;
    void (*pic_mask)(struct pic *, unsigned);
    void (*pic_unmask)(struct pic *, unsigned);
    void (*pic_route)(struct pic *, struct cpu_info *, unsigned, int);
    /* Optional SMP affinity hook (e.g., route to CPU set). */
    void (*pic_set_affinity)(struct pic *, unsigned, const void *);
};

enum {
    PIC_GICV2 = 1,
    PIC_GICV3 = 2,
    PIC_SOFT  = 3,
};

#endif /* _AARCH64_PIC_H_ */
