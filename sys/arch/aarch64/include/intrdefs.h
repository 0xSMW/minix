/*
 * AArch64 interrupt priority and sharing definitions (MI interface).
 */
#ifndef _AARCH64_INTRDEFS_H_
#define _AARCH64_INTRDEFS_H_

/* Interrupt priority levels. Keep ordering compatible with MI expectations. */
#define IPL_NONE        0x0
#define IPL_SOFTCLOCK   0x2
#define IPL_SOFTBIO     0x3
#define IPL_SOFTNET     0x4
#define IPL_SOFTSERIAL  0x5
#define IPL_VM          0x6
#define IPL_SCHED       0x7
#define IPL_HIGH        0x8
#define NIPL            9

/* Interrupt sharing types. */
#define IST_NONE        0   /* none */
#define IST_PULSE       1   /* pulsed */
#define IST_EDGE        2   /* edge-triggered */
#define IST_LEVEL       3   /* level-triggered */

/* Software interrupt slots (for MD implementations). */
#define SIR_SERIAL      29
#define SIR_NET         28
#define SIR_BIO         27
#define SIR_CLOCK       26

/* Size limits used by some MD implementations. */
#define MAX_INTR_SOURCES    64

#endif /* _AARCH64_INTRDEFS_H_ */
