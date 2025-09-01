/* Minimal CPU var declarations for AArch64. */
#ifndef _AARCH64_CPUVAR_H_
#define _AARCH64_CPUVAR_H_

struct cpu_info;
#include <sys/types.h>
#include <sys/device.h>
void    cpu_attach(device_t, cpuid_t);

#endif /* _AARCH64_CPUVAR_H_ */
