#ifndef _AARCH64_IPCCONST_H_
#define _AARCH64_IPCCONST_H_

/* Trap vector identifiers (software-defined for MINIX IPC on AArch64). */
#define KERVEC_INTR 32	/* syscall trap to kernel */
#define IPCVEC_INTR 33	/* ipc trap to kernel  */

/* Register used for secondary status return in IPC receive paths. */
#define IPC_STATUS_REG		x1

#endif  /* _AARCH64_IPCCONST_H_ */

