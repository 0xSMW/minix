/* AArch64-specific clock/timer scaffolding. */

#include "kernel/kernel.h"
#include "kernel/clock.h"
#include "kernel/interrupt.h"
#include "kernel/glo.h"
#include <minix/u64.h>
#include "proto.h"

/* GIC and IRQ support */
void gic_init(void);
void gic_enable_irq(unsigned irq);
unsigned gic_read_irq(void);
void gic_eoi(unsigned irq);

static unsigned tsc_per_ms_local;
static irq_hook_t clock_hook;

static inline uint64_t read_cntfrq(void)
{
    uint64_t v; __asm__ volatile("mrs %0, CNTFRQ_EL0" : "=r"(v)); return v;
}

static inline uint64_t read_cntvct(void)
{
    uint64_t v; __asm__ volatile("mrs %0, CNTVCT_EL0" : "=r"(v)); return v;
}

int init_local_timer(unsigned freq)
{
    /* Use architectural counter as cycle source; real IRQ setup TBD. */
    uint64_t frq = read_cntfrq();
    (void)freq; /* ignore requested frequency for now */
    if (frq == 0) frq = 1000000000ULL; /* 1GHz default */
    tsc_per_ms_local = (unsigned)(frq / 1000ULL);
    return 0;
}

void stop_local_timer(void) { }

void arch_timer_int_handler(void)
{
    /* Program next tick. */
    uint64_t frq = read_cntfrq();
    uint64_t tval = frq / system_hz;
    __asm__ volatile("msr CNTV_TVAL_EL0, %0" :: "r"(tval));
}

void cycles_accounting_init(void)
{
    get_cpu_var(cpu_last_tsc) = 0;
    get_cpu_var(cpu_last_idle) = 0;
}

void context_stop(struct proc * p)
{
    u64_t tsc = read_cntvct();
    u64_t *tsc_sw = get_cpulocal_var_ptr(tsc_ctr_switch);
    u64_t tsc_delta = tsc - *tsc_sw;

    p->p_cycles += tsc_delta;
    *tsc_sw = tsc;
}

void context_stop_idle(void)
{
    context_stop(get_cpulocal_var_ptr(idle_proc));
}

void restart_local_timer(void)
{
    arch_timer_int_handler();
}

int register_local_timer_handler(const irq_handler_t handler)
{
    /* Initialize GIC and hook the virtual timer PPI (27). */
    const int timer_irq = 27; /* ARM Generic Timer Virtual IRQ */

    gic_init();
    put_irq_handler(&clock_hook, timer_irq, handler);
    gic_enable_irq(timer_irq);

    /* Enable virtual timer and set first tick. */
    uint64_t frq = read_cntfrq();
    uint64_t tval = frq / system_hz;
    __asm__ volatile("msr CNTV_CTL_EL0, %0" :: "r"((uint64_t)1));
    __asm__ volatile("msr CNTV_TVAL_EL0, %0" :: "r"(tval));

    /* Globally enable IRQs. */
    interrupts_enable();
    return 0;
}

u64_t ms_2_cpu_time(unsigned ms)
{
    return (u64_t)tsc_per_ms_local * ms;
}

unsigned cpu_time_2_ms(u64_t cpu_time)
{
    return (unsigned long)(cpu_time / tsc_per_ms_local);
}

short cpu_load(void)
{
    return 0; /* TBD */
}
