/* PL011 UART console for QEMU virt (AArch64). */
#include <stdint.h>
#include <stddef.h>

#define UART_BASE   0x09000000ULL
#define UART_DR     0x00
#define UART_FR     0x18
#define UART_IBRD   0x24
#define UART_FBRD   0x28
#define UART_LCRH   0x2C
#define UART_CR     0x30

#define FR_TXFF     (1u << 5)

static inline void mmio_write32(uint64_t addr, uint32_t val)
{ *(volatile uint32_t *)(uintptr_t)addr = val; }
static inline uint32_t mmio_read32(uint64_t addr)
{ return *(volatile uint32_t *)(uintptr_t)addr; }

void bsp_ser_init(void)
{
    /* Disable UART */
    mmio_write32(UART_BASE + UART_CR, 0);
    /* 115200 @ 24MHz: IBRD=13, FBRD≈2 */
    mmio_write32(UART_BASE + UART_IBRD, 13);
    mmio_write32(UART_BASE + UART_FBRD, 2);
    /* 8N1, FIFO enabled */
    mmio_write32(UART_BASE + UART_LCRH, (3 << 5) | (1 << 4));
    /* Enable TX, RX, UART */
    mmio_write32(UART_BASE + UART_CR, (1 << 9) | (1 << 8) | 1);
}

void bsp_ser_putc(char c)
{
    if (c == '\n') {
        bsp_ser_putc('\r');
    }
    while (mmio_read32(UART_BASE + UART_FR) & FR_TXFF) { }
    mmio_write32(UART_BASE + UART_DR, (uint32_t)c);
}

