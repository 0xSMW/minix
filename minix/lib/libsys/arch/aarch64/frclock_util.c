/* Some utility functions around the free running clock on AArch64.
 * This mirrors the ARM implementation; platform wiring provides the
 * free-running clock in minix_kerninfo.
 */

#include <minix/minlib.h>
#include <minix/sysutil.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <lib.h>
#include <assert.h>

#define MICROHZ         1000000ULL /* number of micros per second */
#define MICROSPERTICK(h) (MICROHZ/(h)) /* number of micros per HZ tick */

static u64_t Hz;

int
micro_delay(u32_t micros)
{
    struct minix_kerninfo *minix_kerninfo;
    u64_t start, delta, delta_end;

    Hz = sys_hz();
    minix_kerninfo = get_minix_kerninfo();

    /* Start of delay. */
    read_frclock_64(&start);
    assert(minix_kerninfo->arm_frclock);
    assert(minix_kerninfo->arm_frclock->hz);
    delta_end = (minix_kerninfo->arm_frclock->hz * micros) / MICROHZ;

    /* Use tickdelay for at least one HZ tick, then busywait. */
    if (micros >= MICROSPERTICK(Hz))
        tickdelay(micros*Hz/MICROHZ);

    do {
        read_frclock_64(&delta);
    } while (delta_frclock_64(start, delta) < delta_end);

    return 0;
}

u32_t frclock_64_to_micros(u64_t tsc)
{
    return (u32_t)
        (tsc / (get_minix_kerninfo()->arm_frclock->hz / MICROHZ));
}

void
read_frclock(u32_t *frclk)
{
    struct minix_kerninfo *minix_kerninfo = get_minix_kerninfo();

    assert(frclk);
    assert(minix_kerninfo->arm_frclock);
    assert(minix_kerninfo->arm_frclock->tcrr);
    *frclk = *(volatile u32_t *)((u8_t *)
        minix_kerninfo->arm_frclock->tcrr);
}

u32_t
delta_frclock(u32_t base, u32_t cur)
{
    u32_t delta;

    if (cur < base) {
        delta = (UINT_MAX - base) + cur;
    } else {
        delta = cur - base;
    }

    return delta;
}

void
read_frclock_64(u64_t *frclk)
{
    read_frclock((u32_t *) frclk);
}

u64_t
delta_frclock_64(u64_t base, u64_t cur)
{
    return (u64_t) delta_frclock((u32_t) base, (u32_t) cur);
}

