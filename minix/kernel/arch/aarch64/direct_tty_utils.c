#include "kernel/kernel.h"
#include "direct_utils.h"
#include "include/bsp_serial.h"
#include "glo.h"

void direct_cls(void)
{
    /* Do nothing */
}

void direct_print_char(char c)
{
    bsp_ser_putc(c);
}

void direct_print(const char *str)
{
    while (*str) {
        direct_print_char(*str++);
    }
}

int direct_read_char(unsigned char *ch)
{
    (void)ch; return 0;
}

