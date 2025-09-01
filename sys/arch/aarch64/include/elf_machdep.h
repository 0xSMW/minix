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

/*
 * Processor specific relocation types (subset used by rtld and linkers)
 */
#define R_AARCH64_NONE       0
#define R_AARCH64_COPY       1024
#define R_AARCH64_GLOB_DAT   1025
#define R_AARCH64_JUMP_SLOT  1026
#define R_AARCH64_RELATIVE   1027

#define R_TYPE(name)         __CONCAT(R_AARCH64_,name)

#endif /* _AARCH64_ELF_MACHDEP_H_ */
