/* Busy-wait spin helpers (AArch64) -- mirrored from ARM implementation. */
#include "sysutil.h"
#include <minix/spin.h>
#include <minix/minlib.h>

#define TSC_SPIN        1000    /* in microseconds */

enum {
    STATE_INIT,
    STATE_BASE_TS,
    STATE_TS,
    STATE_UPTIME
};

void spin_init(spin_t *s, u32_t usecs)
{
    s->s_state = STATE_INIT;
    s->s_usecs = usecs;
    s->s_timeout = FALSE;
}

int spin_check(spin_t *s)
{
    u64_t cur_tsc, tsc_delta;
    clock_t now, micro_delta;

    switch (s->s_state) {
    case STATE_INIT:
        s->s_state = STATE_BASE_TS;
        break;
    case STATE_BASE_TS:
        s->s_state = STATE_TS;
        read_frclock_64(&s->s_base_tsc);
        break;
    case STATE_TS:
        read_frclock_64(&cur_tsc);
        tsc_delta = delta_frclock_64(s->s_base_tsc, cur_tsc);
        micro_delta = frclock_64_to_micros(tsc_delta);
        if (micro_delta >= s->s_usecs) {
            s->s_timeout = TRUE;
            return FALSE;
        }
        if (micro_delta >= TSC_SPIN) {
            s->s_usecs -= micro_delta;
            s->s_base_uptime = getticks();
            s->s_state = STATE_UPTIME;
        }
        break;
    case STATE_UPTIME:
        now = getticks();
        micro_delta = ((now - s->s_base_uptime) * 1000 / sys_hz()) * 1000;
        if (micro_delta >= s->s_usecs) {
            s->s_timeout = TRUE;
            return FALSE;
        }
        break;
    default:
        panic("spin_check: invalid state %d", s->s_state);
    }
    return TRUE;
}

