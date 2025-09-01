/* Minimal AArch64 mcontext to satisfy userland build. */

#ifndef _AARCH64_MCONTEXT_H_
#define _AARCH64_MCONTEXT_H_

#include <sys/stdint.h>

/* General register state (x0-x30, sp, pc, pstate) */
#define _NGREG 34
typedef uint64_t __greg_t;
typedef __greg_t __gregset_t[_NGREG];

/* Register indices */
#define _REG_X0  0
#define _REG_SP  31
#define _REG_PC  32

typedef struct {
    __gregset_t __gregs;
#if defined(__minix)
    int mc_flags;
    int mc_magic;
#else
    __greg_t _mc_tlsbase;
#endif
} mcontext_t;

/* Accessors used by generic ucontext helpers */
#define _UC_MACHINE_SP(uc)     ((uc)->uc_mcontext.__gregs[_REG_SP])
#define _UC_MACHINE_PC(uc)     ((uc)->uc_mcontext.__gregs[_REG_PC])
#define _UC_MACHINE_INTRV(uc)  ((uc)->uc_mcontext.__gregs[_REG_X0])

/* TLS base presence flag for ports that use it */
#define _UC_TLSBASE 0x00080000

#endif /* _AARCH64_MCONTEXT_H_ */

