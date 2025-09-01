#include <machine/asm.h>
#include "arch_proto.h"

.section .startup
.global MINIX
MINIX:
    // AArch64 minimal entry: set up a temporary stack and call pre_init, then kmain
    ldr x9, =load_stack_start
    mov sp, x9
    mov x29, xzr       // clear frame pointer
    bl  _C_LABEL(pre_init)
    // Pass &kinfo to kmain
    ldr x0, =_C_LABEL(kinfo)
    bl  _C_LABEL(kmain)

1:  b   1b

.data
.balign 16
load_stack:
    .space 4096
load_stack_start:
