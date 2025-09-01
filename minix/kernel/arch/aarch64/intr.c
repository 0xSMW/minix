#include "kernel/kernel.h"
#include "include/hw_intr.h"

int intr_init(const int auto_eoi)
{
    (void)auto_eoi;
    hw_intr_disable_all();
    return OK;
}

