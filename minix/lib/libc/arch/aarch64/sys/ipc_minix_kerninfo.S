#include <minix/ipcconst.h>
#include <machine/asm.h>

ENTRY(ipc_minix_kerninfo)
    /* x0: pointer to output struct location */
    mov     x9, x0          /* save out ptr */
    mov     x1, #0
    mov     x2, #0
    mov     x0, #MINIX_KERNINFO
    mov     x3, #IPCVEC_INTR
    svc     #0
    str     x1, [x9]
    ret

