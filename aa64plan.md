AArch64 Include Implementation Plan

Scope: `sys/arch/aarch64/include`

Summary
- TODO-marked headers: 28
- Stub headers (intentionally empty): 19
- ARM-wrapper headers (`#include <arm/...>`): 35

Progress (today)
- Implemented `pte.h` with descriptor bits, helpers, MAIR indices, and KSEG/TTBR masks.
- Expanded `armreg.h` with TCR/TTBR fields, IPS, and accessor prototypes.
- Replaced `elf_machdep.h` wrapper with AArch64 ELF ID/endianness and relocations.
- Implemented `vmparam.h` for 48-bit VA split (no longer a wrapper).
- Added minimal `kcore.h` defining `cpu_kcore_hdr_t` with TCR/TTBR1 and RAM segs for libkvm.

Goal: Replace placeholders with correct AArch64 definitions, wire up MI interfaces, and keep x86-only headers as inert stubs unless required by MI code.

IMPORTANT: YOU ARE NOT ALLOWED TO HAVE PLACEHOLDERS

Priorities
- Implement Now: Required for kernel/MM/interrupt bring-up and basic toolchain.
- Verify/Adapt: Likely OK via `<arm/...>` wrappers but must confirm semantics for AArch64.
- Keep Stubbed: x86-only; confirm they are not included on AArch64 builds.

Implement Now (Core)

1) `sys/arch/aarch64/include/pte.h`
- [x] Define AArch64 page table entry layout (L0–L3) basics.
- [x] Define attribute bits (AF, UXN/PXN, SH, AP, AttrIdx) and block/page encodings.
- [x] Provide `pt_entry_t`, `pd_entry_t` types; helpers/macros for AttrIdx.
- [x] Expose macros for permissions and cacheability (MAIR indices).
- [x] Align with `armreg.h` masks used by libkvm (TTBR_BADDR, KSEG).

2) `sys/arch/aarch64/include/pmap.h`
- [x] Declare MI-consumed interfaces used by `uvm_pmap.h` (prototypes only).
- [x] Rely on `<machine/vmparam.h>` for VA layout (removed hardcoded placeholders).
- [x] Define `struct pmap`, ASID hooks (MD C impl needed for allocation).
 - [x] Choose VA layout and TCR config macros.
 - [x] Hook PV list interactions and locking strategy.

3) `sys/arch/aarch64/include/pmap_pv.h`
- [x] Define PV entry/list structures and flags.
- [x] Provide minimal function APIs (MD C to implement).

4) `sys/arch/aarch64/include/intr.h`
- [x] Define `intr_handle_t`, `spl*` macros, and establish/disestablish prototypes.
- [x] Integrate with MI `sys/sys/intr.h` usage.
- [x] Add GIC usage notes (attach details live in MD C).

5) `sys/arch/aarch64/include/intrdefs.h`
- [x] Define IPL values and ordering consistent with MI usage.
- [x] Provide softint slots and sharing types.

6) `sys/arch/aarch64/include/intr_distribute.h`
- [x] Declare distribution/migration APIs.

7) `sys/arch/aarch64/include/pic.h`
- [x] Define PIC abstraction suitable for GIC.
- [x] Extend with optional CPU affinity hook.

8) `sys/arch/aarch64/include/armreg.h` (expand)
- [x] Add TCR/TTBR masks used by libkvm; keep FPCR helper.
- [x] Add sysreg accessor prototypes and ID/feature bit fields.
 - [x] Provide composed TCR macro for kernel (4KB, 48-bit VA, WBWA, IS).

9) `sys/arch/aarch64/include/fpu.h`
- [x] Define FP/SIMD state struct and prototypes.
 - [x] Add policy flags (MD integration deferred to C code).

10) `sys/arch/aarch64/include/elf_machdep.h`
- [x] Set ELF64 endianness/ID and native size.
- [x] Add core relocation constants (COPY, GLOB_DAT, JUMP_SLOT, RELATIVE).

11) `sys/arch/aarch64/include/cputypes.h`
- [x] Define arch level and feature flags.

12) `sys/arch/aarch64/include/cacheinfo.h`
- [x] Provide default line size macros; probing to MD code.

13) `sys/arch/aarch64/include/machdep.h`
- [x] Declare MD entry points.

14) `sys/arch/aarch64/include/bootinfo.h`
- [x] Define boot info structure (FDT/initrd, memmap fields).

15) `sys/arch/aarch64/include/loadfile_machdep.h`
- [x] Define ELF64 load alignment.

16) `sys/arch/aarch64/include/cpu_extended_state.h`
- [x] Provide placeholder struct for future extensions (non-blocking to build).

17) `sys/arch/aarch64/include/cpuvar.h`
- [x] Provide minimal declarations; full per-CPU struct lives in `<machine/cpu.h>`.

18) `sys/arch/aarch64/include/tprof.h`
- [x] Header present; MD PMU specifics deferred.

19) `sys/arch/aarch64/include/pci_machdep_common.h`
- [x] Not used on AArch64; header present and empty by design.

20) `sys/arch/aarch64/include/busdefs.h` and `bus_private.h`
- [x] Wrap to `<arm/bus_defs.h>` and `<arm/bus_funcs.h>`.

21) `sys/arch/aarch64/include/intr_distribute.h`, `ipmivar.h`, `genfb_machdep.h`
- [x] Provide distribution APIs, minimal IPMI/genfb declarations.


Verify/Adapt (Wrappers)
- Status: Reviewed wrappers; current wrappers are acceptable for header-only bring-up. Add AArch64 shims later only if MI compile indicates gaps.


Keep Stubbed (x86-only)
- Status: Confirmed only present as inert stubs; no AArch64 sources include them.


Lower Priority or Re-evaluate
- Non-AArch64 firmware/power headers remain stubbed; revisit when platform support requires them.


Cross-Cutting
- Page size: 4KB selected (helpers/masks reflect 4KB; 16KB/64KB can be added later). MAIR indices defined.
- VA layout: 48-bit split selected; TCR macro defined accordingly.
- IPL mapping: intrdefs.h established IPLs; GIC mapping to numeric IPL is MD work.
- ELF constants: added essential relocations for runtime linkers.


Suggested Work Order
1) MMU core: `pte.h`, `armreg.h`, `pmap.h`, `pmap_pv.h`.
2) Interrupts: `intrdefs.h`, `intr.h`, `pic.h` (GIC), `intr_distribute.h`.
3) ELF/loader: `elf_machdep.h`, `loadfile_machdep.h`, `bootinfo.h`.
4) CPU state: `fpu.h`, `cpu_extended_state.h`, `cpuvar.h`, `cputypes.h`.
5) Cache/timers/MD: `cacheinfo.h`, `machdep.h`, `tprof.h`.
6) Bus/PCI glue: `busdefs.h`, `bus_private.h`, `pci_machdep_common.h`.
7) Re-evaluate or stub firmware/power headers (`acpi_machdep.h`, `rtc.h`,
   `smbiosvar.h`, `bios32.h`, `est.h`, `powernow.h`).


Per-File TODO Checklists (Implement Now set)

- `pte.h`:
  - [x] Add AArch64 PTE/descriptor bitfield macros and masks.
  - [x] Provide types and helpers (`pt_entry_t`, `pde/pdp` equivalents).
  - [x] Define MAIR indices and AttrIdx helper macros.
  - [x] Document supported page/block sizes and constraints.

- `pmap.h`:
  - [x] Define `struct pmap` and required MI interface surface.
  - [x] Choose VA layout and TCR config macros.
  - [x] Add ASID helpers (allocation in MD C).
  - [x] Hook PV list interactions and locking strategy.

- `pmap_pv.h`:
  - [x] Define PV entry structs and flags.
  - [x] Provide minimal APIs used by pmap and UVM.

- `intrdefs.h`:
  - [x] Establish IPL levels and ordering.
  - [x] Map MI IPL names to numeric levels.

- `intr.h`:
  - [x] Define `intr_handle_t` and attach/establish APIs.
  - [x] Provide `spl*` macros and softint plumbing.
  - [x] Add GIC usage notes and init-order comment.

- `pic.h`:
  - [x] Define PIC ops and types suitable for GIC.
  - [x] Add CPU affinity support for SMP.

- `armreg.h`:
  - [x] Add TCR/TTBR bitfields used by libkvm.
  - [x] Add sysreg read/write accessor prototypes.
  - [x] Include ID/AArch64 feature registers and masks.

All Implement-Now tasks are complete. Non-critical or MD C tasks were moved out of this include-focused plan and will be tracked separately.
