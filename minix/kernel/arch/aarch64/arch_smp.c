/* AArch64 SMP scaffolding using PSCI (UP fallback). */

#include "kernel/kernel.h"
#include "kernel/smp.h"

/* Minimal halt: drop into WFI loop */
void arch_smp_halt_cpu(void)
{
    BKL_UNLOCK();
    for(;;) __asm__ volatile("wfi");
}

/* No-op schedule IPI in UP mode */
void arch_send_smp_schedule_ipi(unsigned cpu)
{
    (void)cpu;
}

/* Discover/bring up APs via PSCI in the future; for now, UP fallback */
void smp_init(void)
{
    bsp_cpu_id = 0;
    ncpus = 1;
}

