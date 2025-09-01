/*
 * AArch64 ELF machine definitions (minimal set to satisfy MI consumers).
 */
#ifndef _AARCH64_ELF_MACHDEP_H_
#define _AARCH64_ELF_MACHDEP_H_

/* AArch64 is typically little-endian; big-endian variants not enabled here. */
#define ELF32_MACHDEP_ENDIANNESS	ELFDATA2LSB
#define ELF64_MACHDEP_ENDIANNESS	ELFDATA2LSB

/* Identify AArch64 binaries */
#define	ELF64_MACHDEP_ID_CASES \
		case EM_AARCH64: \
			break;

#define ELF64_MACHDEP_ID	EM_AARCH64

/* Native binary size */
#define ARCH_ELFSIZE		64

#endif /* _AARCH64_ELF_MACHDEP_H_ */
