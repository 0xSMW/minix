/* AArch64 system-dependent kernel functions (scaffold). */

#include "kernel/kernel.h"

#include <string.h>
#include <assert.h>
#include <minix/u64.h>
#include <machine/vm.h>

#include "archconst.h"
#include "arch_proto.h"
#include "kernel/proc.h"
#include "kernel/debug.h"
#include "glo.h"

void * k_stacks;

void fpu_init(void) { }
void save_local_fpu(struct proc *pr, int retain) { (void)pr; (void)retain; }
void save_fpu(struct proc *pr) { (void)pr; }
int restore_fpu(struct proc *pr) { (void)pr; return 0; }

void arch_proc_reset(struct proc *pr)
{
    assert(pr->p_nr < NR_PROCS);
    memset(&pr->p_reg, 0, sizeof(pr->p_reg));
    /* PSTATE defaults handled on return to user */
}

void arch_proc_setcontext(struct proc *p, struct stackframe_s *state,
    int isuser, int trapstyle)
{
    (void)isuser; (void)trapstyle;
    assert(sizeof(p->p_reg) == sizeof(*state));
    if (state != &p->p_reg) memcpy(&p->p_reg, state, sizeof(*state));
    p->p_misc_flags |= MF_CONTEXT_SET;
}

void arch_set_secondary_ipc_return(struct proc *p, u32_t val)
{
    p->p_reg.r1 = (reg_t)val;
}

static inline uint64_t read_midr_el1(void)
{
    uint64_t v; __asm__ volatile("mrs %0, MIDR_EL1" : "=r"(v)); return v;
}

void cpu_identify(void)
{
    uint64_t midr = read_midr_el1();
    unsigned cpu = cpuid;
    cpu_info[cpu].implementer = (midr >> 24) & 0xFF;
    cpu_info[cpu].variant = (midr >> 20) & 0xF;
    cpu_info[cpu].arch = (midr >> 16) & 0xF;
    cpu_info[cpu].part = (midr >> 4) & 0xFFF;
    cpu_info[cpu].revision = midr & 0xF;
    cpu_info[cpu].freq = 1000; /* placeholder MHz */
}

void arch_init(void)
{
    k_stacks = (void*) &k_stacks_start;
    /* Initialize serial early so printf works. */
    extern void bsp_ser_init(void);
    bsp_ser_init();
    extern void dt_init(void);
    dt_init();
}

void do_ser_debug(void) { }

void arch_do_syscall(struct proc *proc)
{
    assert(proc == get_cpulocal_var(proc_ptr));
    proc->p_reg.retreg =
        do_ipc(proc->p_reg.retreg, proc->p_reg.r1, proc->p_reg.r2);
}

struct proc * arch_finish_switch_to_user(void)
{
    struct proc *p = get_cpulocal_var(proc_ptr);
    /* clear interrupt mask bits so user runs with IRQs enabled */
    p->p_reg.psr &= ~(0ULL);
    return p;
}

void fpu_sigcontext(struct proc *pr, struct sigframe_sigcontext *fr, struct sigcontext *sc)
{ (void)pr; (void)fr; (void)sc; }

reg_t arch_get_sp(struct proc *p) { return p->p_reg.sp; }

void get_randomness(struct k_randomness *rand, int source)
{ (void)rand; (void)source; }

/* Address space switch stub; full TTBR handling TBD. */
void __switch_address_space(struct proc *p, struct proc **__ptproc)
{
    (void)p;
    *__ptproc = p;
}

void arch_ser_init(void) { bsp_ser_init(); }
