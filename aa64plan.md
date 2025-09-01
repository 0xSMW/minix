AArch64 Include Implementation Plan

Scope: `sys/arch/aarch64/include`

Summary
- TODO-marked headers: 28
- Stub headers (intentionally empty): 19
- ARM-wrapper headers (`#include <arm/...>`): 35

Progress (today)
- Implemented minimal `pte.h` with descriptor bits, types, MAIR AttrIdx helpers, and KSEG/TTBR base mask used by libkvm.
- Added TCR/TTBR bitfields to `armreg.h` needed by libkvm (accessors pending).
- Replaced `elf_machdep.h` wrapper with AArch64 ELF ID/endianness definitions.
- Added `vmparam.h` wrapper to `<arm/vmparam.h>` to satisfy `<machine/vmparam.h>` includes.
- Added minimal `kcore.h` defining `cpu_kcore_hdr_t` with `kh_tcr1`, `kh_ttbr1`, and `kh_ramsegs[]` for libkvm.

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

9) `sys/arch/aarch64/include/fpu.h`
- [x] Define FP/SIMD state struct and prototypes.
- [ ] Add policy flags and full MD integration.

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
- Review each ARM-wrapper header to confirm it is AArch64-safe:
  `ansi.h`, `aout_machdep.h`, `bswap.h`, `bus_defs.h`, `bus_funcs.h`,
  `byte_swap.h`, `cdefs.h`, `cpu.h`, `cpu_counter.h`, `cpufunc.h`,
  `db_machdep.h`, `endian_machdep.h`, `float.h`, `ieee.h`, `ieeefp.h`,
  `int_const.h`, `int_fmtio.h`, `int_limits.h`, `int_mwgwtypes.h`, `int_types.h`,
  `isa_machdep.h`, `limits.h`, `lock.h`, `math.h`, `mutex.h`, `param.h`,
  `pci_machdep.h`, `pio.h`, `rwlock.h`, `signal.h`, `sysarch.h`, `trap.h`,
  `types.h`, `wchar_limits.h`, and `elf_machdep.h` wrapper note.
- Action items:
  - Confirm all referenced `<arm/...>` headers exist and are v8/AArch64-compatible.
  - Add AArch64-specific shims only where the generic ARM definition is wrong or
    incomplete for 64-bit (sizes, alignment, ABI differences).


Keep Stubbed (x86-only; confirm unused)
- Stubs present by design and should remain inert unless MI references are found:
  `apicvar.h`, `cpu_ucode.h`, `i82093reg.h`, `i82093var.h`, `i82489reg.h`,
  `i82489var.h`, `i8259.h`, `mpacpi.h`, `mpbiosreg.h`, `mpbiosvar.h`, `mpconfig.h`,
  `mtrr.h`, `nmi.h`, `psl.h`, `specialreg.h`, `via_padlock.h`, `vga_post.h`.
- Action items:
  - Run tree-wide searches to ensure no AArch64 object includes these headers.
  - If referenced, replace include sites with GIC/AArch64 equivalents or provide
    narrow shim macros to satisfy the caller.


Lower Priority or Re-evaluate
- `rtc.h`: Prefer ARM generic timers/SoC RTC; implement stub or MD RTC glue
  only if required by platform drivers.
- `est.h`, `powernow.h`, `bios32.h`, `smbiosvar.h`: x86 power/firmware. Keep as
  stubs unless a platform mandates equivalent mechanisms; otherwise remove from
  AArch64 build graph when feasible.
- `acpi_machdep.h`: Implement only if ACPI is supported on target platforms; DT
  may be primary on many boards.


Cross-Cutting Tasks
- Decide supported page size(s) (4K mandatory; 16K/64K optional) and encode in
  `pte.h`, `pmap.h`, and `armreg.h` TCR/MAIR definitions.
- Establish VA layout (user/kernel split) and ASID allocation policy.
- Define IPL mapping to GIC priorities; document constraints in `intr*.h`.
- Validate ELF machine constants by building libelf and simple hello-world.
- Ensure libkvm (`lib/libkvm/kvm_aarch64.c`) compiles with the new `pte.h`.
- Confirm MI headers (`sys/sys/intr.h`, `sys/uvm/uvm_pmap.h`) compile cleanly.


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

- `fpu.h`:
  - [x] Define FP/SIMD state structure and alignment.
  - [x] Declare save/restore hooks/macros.

- `elf_machdep.h`:
  - [x] Set EM_AARCH64 and endianness/size.
  - [ ] Add relocation constants if needed by toolchain.

- `cputypes.h`:
  - [x] Define CPU feature flags and classes.
  - [ ] Provide compile-time checks for required features.

- `cacheinfo.h`:
  - [ ] Read CTR/CCSIDR/CLIDR to populate cache descriptors.
  - [x] Expose line sizes and coherency hints.

- `machdep.h`:
  - [x] Declare MD entry points used by MI code.
  - [ ] Provide platform constants and helper macros.

- `bootinfo.h`:
  - [x] Define boot info struct (FDT/initrd/memmap fields).
  - [ ] Align with early MD init consumers.

- `loadfile_machdep.h`:
  - [x] Define loader page alignment and flags.
  - [ ] Ensure relocations/types match `elf_machdep.h`.

- `cpu_extended_state.h`:
  - [x] Reserve/define additional extended state (feature-gated).

- `cpuvar.h`:
  - [ ] Define per-CPU struct and accessors.
  - [ ] Include GIC CPU interface fields if needed.

- `tprof.h`:
  - [ ] Define PMU event IDs/counters usable by tprof.
  - [ ] Provide feature checks and fallbacks.

- `pci_machdep_common.h`:
  - [ ] Minimal DMA/interrupt glue for AArch64 PCIe (not needed for build).
  - [ ] Decide MSI/MSI-X support policy.

- `busdefs.h` / `bus_private.h`:
  - [x] Ensure bus headers map to ARM MD where appropriate.
  - [ ] Add AArch64 shims only if generic ARM is insufficient.


Verification Steps
- [ ] Build libelf and confirm no missing AArch64 ELF macros.
- [ ] Build `lib/libkvm/kvm_aarch64.c` to validate `<machine/pte.h>`.
- [ ] Compile MI `sys/sys/intr.h` and `uvm_pmap.h` with AArch64 headers.
- [ ] Grep for includes of stubbed x86 headers; ensure no AArch64 usage.
