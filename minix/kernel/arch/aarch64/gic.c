/* Very small GICv2 scaffold for QEMU virt platform (non-functional stub). */
#include "kernel/kernel.h"

#define GICD_BASE   0x08000000ULL
#define GICC_BASE   0x08010000ULL

#define GICD_CTLR   0x000
#define GICD_ISENABLER(n) (0x100 + 4*(n))
#define GICD_ICENABLER(n) (0x180 + 4*(n))

#define GICC_CTLR   0x000
#define GICC_PMR    0x004
#define GICC_IAR    0x00C
#define GICC_EOIR   0x010

static inline void mmio_write32(uint64_t addr, uint32_t val)
{ *(volatile uint32_t *)(uintptr_t)addr = val; }
static inline uint32_t mmio_read32(uint64_t addr)
{ return *(volatile uint32_t *)(uintptr_t)addr; }

void gic_init(void)
{
    /* enable distributor */
    mmio_write32(GICD_BASE + GICD_CTLR, 1);
    /* enable CPU interface, set priority mask */
    mmio_write32(GICC_BASE + GICC_PMR, 0xFF);
    mmio_write32(GICC_BASE + GICC_CTLR, 1);
}

void gic_enable_irq(unsigned irq)
{
    unsigned n = irq / 32, b = irq % 32;
    mmio_write32(GICD_BASE + GICD_ISENABLER(n), (1u << b));
}

void gic_disable_irq(unsigned irq)
{
    unsigned n = irq / 32, b = irq % 32;
    mmio_write32(GICD_BASE + GICD_ICENABLER(n), (1u << b));
}

unsigned gic_read_irq(void)
{
    return mmio_read32(GICC_BASE + GICC_IAR) & 0x3FF;
}

void gic_eoi(unsigned irq)
{
    mmio_write32(GICC_BASE + GICC_EOIR, irq);
}

