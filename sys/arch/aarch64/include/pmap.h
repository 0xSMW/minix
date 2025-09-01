/*
 * AArch64 pmap interface — minimal MD surface to satisfy MI headers.
 */
#ifndef _AARCH64_PMAP_H_
#define _AARCH64_PMAP_H_

#include <sys/types.h>
/* Avoid including UVM headers here to prevent cycles. */

/*
 * ASID handling (16-bit ASIDs on AArch64).
 */
typedef uint16_t asid_t;
#define PMAP_ASID_INVALID   ((asid_t)0)

struct pmap {
    volatile u_int    pm_refcnt;   /* references to this pmap */
    asid_t            pm_asid;     /* hardware ASID for this pmap */
    void             *pm_l0;       /* L0 (top-level) table VA */
    long              pm_resident; /* # of resident pages */
    long              pm_wired;    /* # of wired pages */
};
typedef struct pmap *pmap_t;

/* VM address layout comes from <machine/vmparam.h>. */

/* Forward declarations for MI prototypes in uvm_pmap.h */
#ifdef _KERNEL
#include <uvm/pmap/vmpagemd.h>
struct vm_page;
struct pmap *   pmap_create(void);
void            pmap_destroy(pmap_t);
int             pmap_enter(pmap_t, vaddr_t, paddr_t, vm_prot_t, u_int);
bool            pmap_extract(pmap_t, vaddr_t, paddr_t *);
void            pmap_init(void);
void            pmap_kenter_pa(vaddr_t, paddr_t, vm_prot_t, u_int);
void            pmap_kremove(vaddr_t, vsize_t);
void            pmap_page_protect(struct vm_page *, vm_prot_t);
void            pmap_protect(pmap_t, vaddr_t, vaddr_t, vm_prot_t);
void            pmap_reference(pmap_t);
void            pmap_remove(pmap_t, vaddr_t, vaddr_t);
void            pmap_remove_all(struct pmap *);
void            pmap_update(pmap_t);
long            pmap_resident_count(pmap_t);
long            pmap_wired_count(pmap_t);
void            pmap_zero_page(paddr_t);
void            pmap_virtual_space(vaddr_t *, vaddr_t *);

/* ASID helpers (implemented in MD code). */
asid_t          pmap_asid_alloc(pmap_t);
void            pmap_asid_release(pmap_t);
#endif

#endif /* _AARCH64_PMAP_H_ */
