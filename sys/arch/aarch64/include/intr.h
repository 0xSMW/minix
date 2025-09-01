/*
 * AArch64 interrupt interface (headers only; MD implementation elsewhere).
 *
 * NOTE: Typical systems use ARM GICv2/v3. The MD code is responsible for
 * wiring the root PIC, establishing priorities that map to IPL_* values,
 * and distributing SPIs/PPIs across CPUs. This header keeps only the
 * MI-facing declarations and softint plumbing.
 */
#ifndef _AARCH64_INTR_H_
#define _AARCH64_INTR_H_

#include <sys/types.h>
#include <machine/intrdefs.h>

#ifdef _KERNEL

/* Optional MD capabilities used by MI code. */
#define __HAVE_FAST_SOFTINTS

/* SPL operations (implemented in MD code). */
void    spllower(int);
int     splraise(int);

typedef uint8_t ipl_t;
typedef struct { ipl_t _ipl; } ipl_cookie_t;

static inline ipl_cookie_t makeiplcookie(ipl_t ipl) { return (ipl_cookie_t){._ipl = ipl}; }
static inline int splraiseipl(ipl_cookie_t icookie) { return splraise(icookie._ipl); }

#define spl0()          spllower(IPL_NONE)
#define splx(x)         spllower(x)

/* Interrupt handle type for bus code. */
typedef uint64_t intr_handle_t;

/* MD attach/detach helpers (stubs declared here). */
struct intrhand;
void *  intr_establish_xname(int, void *, int, int, int, int (*)(void *), void *, bool, const char *);
void *  intr_establish(int, void *, int, int, int, int (*)(void *), void *, bool);
void    intr_disestablish(struct intrhand *);

#include <sys/spl.h>

#endif /* _KERNEL */

#endif /* _AARCH64_INTR_H_ */
