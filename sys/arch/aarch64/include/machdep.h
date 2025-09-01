/*
 * AArch64 MD interfaces referenced by MI code.
 */
#ifndef _AARCH64_MACHDEP_H_
#define _AARCH64_MACHDEP_H_

#ifdef _KERNEL
void    cpu_startup(void);
void    cpu_reboot(int, char *);
void    dumpsys(void);
void    delay(unsigned int);
#endif

#endif /* _AARCH64_MACHDEP_H_ */
