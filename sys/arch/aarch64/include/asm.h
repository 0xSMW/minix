/*
 * Minimal AArch64 asm.h providing entry/alias macros used by csu.
 * This tree lacks a full aarch64 kernel port; keep this small and generic.
 */

#ifndef _AARCH64_ASM_H_
#define _AARCH64_ASM_H_

/* Label helpers */
#define _C_LABEL(x) x
#define _ASM_LABEL(x) x

/* Section and type helpers */
#ifndef _TEXT_SECTION
#define _TEXT_SECTION .text
#endif

#ifndef _ALIGN_TEXT
#define _ALIGN_TEXT .p2align 2
#endif

/* GAS function type tokens */
#define _ASM_TYPE_FUNCTION %function

/* Emit common function prologue label */
#define _ENTRY(x) \
	_TEXT_SECTION; _ALIGN_TEXT; .globl x; .type x,_ASM_TYPE_FUNCTION; x:

/* Emit function size */
#define _END(x) \
	.size x, .-x

/* Public API expected by csu */
#define ENTRY(y)   _ENTRY(_C_LABEL(y))
#define ENTRY_NP(y) _ENTRY(_C_LABEL(y))
#define END(y)     _END(_C_LABEL(y))

/* Misc helpers */
#define RCSID(x) \
	.pushsection ".ident"; .asciz x; .popsection

#define WEAK_ALIAS(alias,sym) \
	.weak alias; \
	alias = sym

#define STRONG_ALIAS(alias,sym) \
	.globl alias; \
	alias = sym

#define ASMSTR		.asciz

#endif /* _AARCH64_ASM_H_ */

