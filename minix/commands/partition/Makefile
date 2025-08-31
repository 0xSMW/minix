PROG=	partition
MAN=	partition.8

# We need this to find our partition.h while compiling natively
# on non-Minix.
CPPFLAGS+= -I${NETBSDSRCDIR}/minix/include/arch/${MACHINE_ARCH}/include
# Apple Silicon (aarch64) has no arch/aarch64 headers in this tree yet;
# use the i386 definitions which are sufficient for host-side utilities.
.if ${MACHINE_ARCH} == "aarch64"
CPPFLAGS+= -I${NETBSDSRCDIR}/minix/include/arch/i386/include
.endif

.include <bsd.prog.mk>
