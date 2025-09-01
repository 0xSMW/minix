/* Minimal AArch64 machine setjmp definitions for userland build. */

#ifndef _AARCH64_SETJMP_H_
#define _AARCH64_SETJMP_H_

/* jmp_buf layout slot indices (each slot is _BSD_JBSLOT_T_) */
#define _JB_MAGIC   0
#define _JB_SP      1

#define _JB_X19     2
#define _JB_X20     3
#define _JB_X21     4
#define _JB_X22     5
#define _JB_X23     6
#define _JB_X24     7
#define _JB_X25     8
#define _JB_X26     9
#define _JB_X27     10
#define _JB_X28     11
#define _JB_X29     12
#define _JB_X30     13

#define _JB_TPIDR   14

#define _JB_D8      15
#define _JB_D9      16
#define _JB_D10     17
#define _JB_D11     18
#define _JB_D12     19
#define _JB_D13     20
#define _JB_D14     21
#define _JB_D15     22

#define _JB_SIGMASK 23

/* Ensure jmp_buf is large enough for saved state + signal mask. */
#define _JBLEN      64

/* Magic values used by setjmp/longjmp validation; differ by 1. */
#define _JB_MAGIC_AARCH64__SETJMP  0ULL
#define _JB_MAGIC_AARCH64_SETJMP   1ULL

#endif /* _AARCH64_SETJMP_H_ */
