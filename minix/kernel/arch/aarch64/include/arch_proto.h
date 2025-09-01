#ifndef _AARCH64_PROTO_H
#define _AARCH64_PROTO_H

#include <machine/vm.h>

#define K_STACK_SIZE	(4096)

#ifndef __ASSEMBLY__
/* early boot helpers provided by common kernel */
void pre_init(void);
void kmain(void *);
#endif

#endif

