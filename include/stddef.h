/*	$NetBSD: stddef.h,v 1.17 2013/12/12 17:53:03 matt Exp $	*/

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)stddef.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _STDDEF_H_
#define _STDDEF_H_

/*
 * When building host tools on macOS with libc++ (C++ includes <cstddef>),
 * prefer the system's <stddef.h> to avoid mixing NetBSD C headers with
 * the host C++ standard library headers.
 */
#if defined(__APPLE__) && defined(__cplusplus) && !defined(__minix)
# include_next <stddef.h>
# define __NETBSD_STDDEF_REDIRECTED
#endif

#ifndef __NETBSD_STDDEF_REDIRECTED
#include <sys/cdefs.h>
#if defined(__has_include)
# if __has_include(<sys/featuretest.h>)
#  include <sys/featuretest.h>
# endif
#else
# include <sys/featuretest.h>
#endif
#if defined(__has_include)
# if __has_include(<machine/ansi.h>)
#  include <machine/ansi.h>
# elif __has_include(<aarch64/ansi.h>)
#  include <aarch64/ansi.h>
# elif __has_include(<arm/ansi.h>)
#  include <arm/ansi.h>
# endif
#else
# include <machine/ansi.h>
#endif

#ifdef	_BSD_PTRDIFF_T_
typedef	_BSD_PTRDIFF_T_	ptrdiff_t;
#undef	_BSD_PTRDIFF_T_
#endif

#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_	size_t;
#undef	_BSD_SIZE_T_
#endif

#if defined(_BSD_WCHAR_T_) && !defined(__cplusplus)
typedef	_BSD_WCHAR_T_	wchar_t;
#undef	_BSD_WCHAR_T_
#endif

#if defined(__has_include)
# if __has_include(<sys/null.h>)
#  include <sys/null.h>
# endif
#else
# include <sys/null.h>
#endif

#if __GNUC_PREREQ__(4, 0)
#define	offsetof(type, member)	__builtin_offsetof(type, member)
#elif !defined(__cplusplus)
#define	offsetof(type, member)	((size_t)(unsigned long)(&((type *)0)->member))
#else
#if !__GNUC_PREREQ__(3, 4)
#define __offsetof__(a) a
#endif
#define	offsetof(type, member) __offsetof__((reinterpret_cast<size_t> \
    (&reinterpret_cast<const volatile char &>(static_cast<type *>(0)->member))))
#endif  

#endif /* !__NETBSD_STDDEF_REDIRECTED */

#endif /* _STDDEF_H_ */
