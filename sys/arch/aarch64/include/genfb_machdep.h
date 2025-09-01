/* Generic framebuffer MD hooks for AArch64 (prototypes). */
#ifndef _AARCH64_GENFB_MACHDEP_H_
#define _AARCH64_GENFB_MACHDEP_H_

struct device;
int     aarch64_genfb_cnattach(void);
void    aarch64_genfb_set_console_dev(struct device *);
void    aarch64_genfb_ddb_trap_callback(int);

#endif /* _AARCH64_GENFB_MACHDEP_H_ */
