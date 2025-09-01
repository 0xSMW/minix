/*
 * AArch64 pv-tracking minimal types, kept small for inclusion in vmparam.h.
 */
#ifndef _AARCH64_PMAP_PV_H_
#define _AARCH64_PMAP_PV_H_

#include <sys/queue.h>

struct vm_page;

struct pv_pte {
    struct vm_page *pte_ptp; /* PTP; NULL for kernel mappings */
    vaddr_t pte_va;          /* VA */
};

struct pv_entry {
    struct pv_pte pve_pte;
    LIST_ENTRY(pv_entry) pve_list;
};

struct pmap_page {
    struct pv_pte pp_pte;
    uint8_t pp_flags;
    uint8_t pp_attrs;
};

#define PP_EMBEDDED 1
#define PMAP_PAGE_INIT(pp) /* none */

/* Minimal PV API surface; MD implementations will define these. */
#ifdef _KERNEL
struct pmap;
bool    pmap_pv_enter(struct pmap *, struct vm_page *, vaddr_t);
void    pmap_pv_remove(struct pmap *, struct vm_page *, vaddr_t);
void    pmap_pv_page_init(struct vm_page *);
#endif

#endif /* _AARCH64_PMAP_PV_H_ */
