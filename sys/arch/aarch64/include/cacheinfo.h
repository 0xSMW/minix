/*
 * AArch64 cache info helpers (header-level constants only).
 */
#ifndef _AARCH64_CACHEINFO_H_
#define _AARCH64_CACHEINFO_H_

#include <sys/stdint.h>

/* Default to 64-byte line if not probed. */
#ifndef AARCH64_DCACHE_LINE_SIZE
#define AARCH64_DCACHE_LINE_SIZE 64
#endif
#ifndef AARCH64_ICACHE_LINE_SIZE
#define AARCH64_ICACHE_LINE_SIZE 64
#endif

#endif /* _AARCH64_CACHEINFO_H_ */
