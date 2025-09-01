# MINIX AArch64 IPC/syscall trampolines
HERE=${NETBSDSRCDIR}/minix/lib/libc/arch/${ARCHSUBDIR}/sys
.PATH: ${HERE}

SRCS+=  \
	_do_kernel_call_intr.S \
	_ipc.S \
	ipc_minix_kerninfo.S

