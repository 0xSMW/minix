/* Device Tree hook for AArch64 (stub for QEMU virt).
 * If a bootloader provides an FDT, record its presence.
 */
#include "kernel/kernel.h"
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stdint.h>
#include <string.h>
#include "glo.h"
#include "kernel/debug.h"
#include "proto.h"
#include "kernel/proc.h"
#include "arch_proto.h"

#include <sys/arch/aarch64/include/bootinfo.h>

static aarch64_bootinfo_t bi_local;

void dt_init(void)
{
    /* For now, just detect if the loader passed an FDT pointer through kinfo. */
    /* In a future step, populate UART/GIC base addresses from the FDT. */
    (void)bi_local;
}

