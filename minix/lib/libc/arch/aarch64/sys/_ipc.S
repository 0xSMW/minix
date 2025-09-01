#include <minix/ipcconst.h>
#include <machine/asm.h>

/* AArch64 IPC entry stubs using SVC */

ENTRY(_ipc_send_intr)
    /* x0: dest, x1: msg ptr */
    mov     x2, x1          /* x2 = msg ptr */
    mov     x1, x0          /* x1 = dest */
    mov     x0, #SEND       /* operation */
    mov     x3, #IPCVEC_INTR
    svc     #0
    ret

ENTRY(_ipc_receive_intr)
    /* x0: src, x1: msg ptr, x2: status ptr */
    mov     x9, x2          /* save status ptr */
    mov     x2, x1          /* x2 = msg ptr */
    mov     x1, x0          /* x1 = src */
    mov     x0, #RECEIVE    /* operation */
    mov     x3, #IPCVEC_INTR
    svc     #0
    str     x1, [x9]        /* store status */
    ret

ENTRY(_ipc_sendrec_intr)
    /* x0: src_dest, x1: msg ptr */
    mov     x2, x1          /* x2 = msg ptr */
    mov     x1, x0          /* x1 = src_dest */
    mov     x0, #SENDREC    /* operation */
    mov     x3, #IPCVEC_INTR
    svc     #0
    ret

ENTRY(_ipc_notify_intr)
    /* x0: dest */
    mov     x1, x0          /* x1 = dest */
    mov     x0, #NOTIFY     /* operation */
    mov     x3, #IPCVEC_INTR
    svc     #0
    ret

ENTRY(_ipc_sendnb_intr)
    /* x0: dest, x1: msg ptr */
    mov     x2, x1          /* x2 = msg ptr */
    mov     x1, x0          /* x1 = dest */
    mov     x0, #SENDNB     /* operation */
    mov     x3, #IPCVEC_INTR
    svc     #0
    ret

ENTRY(_ipc_senda_intr)
    /* x0: table, x1: count */
    mov     x2, x0          /* x2 = table */
    /* x1 already holds count */
    mov     x0, #SENDA      /* operation */
    mov     x3, #IPCVEC_INTR
    svc     #0
    ret

