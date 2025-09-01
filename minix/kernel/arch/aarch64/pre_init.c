#define UNPAGED 1

#include "kernel/kernel.h"
#include <assert.h>
#include <string.h>
#include <minix/minlib.h>
#include <minix/const.h>
#include <minix/type.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/reboot.h>
#include "arch_proto.h"
#include "glo.h"

/* to-be-built kinfo struct, diagnostics buffer */
kinfo_t kinfo;
struct kmessages kmessages;

phys_bytes vir2phys(void *addr) { return (phys_bytes) addr; }

/* kernel bss markers from linker script (optional) */
extern char _etext, _edata, _end;

static void get_parameters(kinfo_t *cbi)
{
    memset(&cbi->mbi, 0, sizeof(cbi->mbi));
    cbi->kmess = &kmess;
    cbi->do_serial_debug = 1;
    cbi->serial_debug_baud = 115200;
}

void pre_init(void)
{
    kinfo_t *cbi = &kinfo;
    memset(cbi, 0, sizeof(*cbi));
    get_parameters(cbi);
}

