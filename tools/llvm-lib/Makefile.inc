#	$NetBSD: Makefile.inc,v 1.1 2011/02/06 01:14:12 joerg Exp $

.include "${.PARSEDIR}/../llvm/Makefile.inc"

HOSTLIB=	${LIB}

.include "${LLVM_TOPLEVEL}/lib/Makefile.inc"
.include "${LLVM_TOPLEVEL}/lib/${.CURDIR:T}/Makefile"

# Ensure host tools see nbinclude headers (without invoking nbincludes recipes here)
HOST_CPPFLAGS+=	-I${TOOLDIR}/include
# Avoid pulling NetBSD userland headers into host C++ tool builds on Darwin.
# They conflict with libc++ and Apple SDK headers.
HOST_CPPFLAGS:=	${HOST_CPPFLAGS:N-I${NETBSDSRCDIR}/include:N-I${NETBSDSRCDIR}/sys/arch}

HOST_CPPFLAGS:=	${HOST_CPPFLAGS:N-Wp,-iremap,*}
