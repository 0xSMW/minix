/* Hardware interrupt control (AArch64, QEMU virt) - scaffold. */
#include "include/hw_intr.h"

void hw_intr_mask(int irq)   { (void)irq; }
void hw_intr_unmask(int irq) { (void)irq; }
void hw_intr_ack(int irq)    { (void)irq; }
void hw_intr_used(int irq)   { (void)irq; }
void hw_intr_not_used(int irq) { (void)irq; }
void hw_intr_disable_all(void) { }

