#include "kernel/kernel.h"

#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/reboot.h>

#include "arch_proto.h"

static inline u64_t smc_call(u64_t fid, u64_t arg0, u64_t arg1, u64_t arg2)
{
    register u64_t x0 __asm__("x0") = fid;
    register u64_t x1 __asm__("x1") = arg0;
    register u64_t x2 __asm__("x2") = arg1;
    register u64_t x3 __asm__("x3") = arg2;
    __asm__ volatile("smc #0" : "+r"(x0) : "r"(x1), "r"(x2), "r"(x3) : "x4","x5","x6","x7","memory");
    return x0;
}

static void psci_system_off(void)
{
    /* PSCI 0.2+ SYSTEM_OFF: 0x84000008 */
    (void)smc_call(0x84000008ULL, 0, 0, 0);
}

static void psci_system_reset(void)
{
    /* PSCI 0.2+ SYSTEM_RESET: 0x84000009 */
    (void)smc_call(0x84000009ULL, 0, 0, 0);
}

void halt_cpu(void)
{
    __asm__ volatile("dsb sy\n\tmsr daifclr, #2\n\twfi\n\tmsr daifset, #2" ::: "memory");
}

static void poweroff(void)
{
    psci_system_off();
    /* Fallback: hang */
    for(;;) halt_cpu();
}

static void reset(void)
{
    psci_system_reset();
    /* Fallback: hang */
    for(;;) halt_cpu();
}

__dead void arch_shutdown(int how)
{
    if ((how & RB_POWERDOWN) == RB_POWERDOWN) {
        poweroff();
        NOT_REACHABLE;
    }

    if (how & RB_HALT) {
        for(;;) halt_cpu();
        NOT_REACHABLE;
    }

    reset();
    NOT_REACHABLE;
}

