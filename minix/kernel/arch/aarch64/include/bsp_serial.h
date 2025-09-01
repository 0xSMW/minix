#ifndef _AARCH64_BSP_SERIAL_H_
#define _AARCH64_BSP_SERIAL_H_

void bsp_ser_init(void);
void bsp_ser_putc(char c);

static inline void ser_putc(char c) { bsp_ser_putc(c); }

#endif

