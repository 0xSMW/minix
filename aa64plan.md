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

Priorities
- Implement Now: Required for kernel/MM/interrupt bring-up and basic toolchain.
- Verify/Adapt: Likely OK via `<arm/...>` wrappers but must confirm semantics for AArch64.
- Keep Stubbed: x86-only; confirm they are not included on AArch64 builds.

Implement Now (Core)

1) `sys/arch/aarch64/include/pte.h`
- Define AArch64 page table entry layout (L0–L3) for 4K pages.
- Define attribute bits (AF, UXN/PXN, SH, AP, AttrIdx) and block/page encodings.
- Provide `pt_entry_t`, `pd_entry_t` types and helpers for entry encode/decode.
- Expose macros for kernel/user permissions and cacheability (MAIR indices).
- Align with `pmap.h` and `armreg.h` register programming.

2) `sys/arch/aarch64/include/pmap.h`
- Define `struct pmap`, kernel pmap globals, and lock strategy.
- Provide VA layout constants (KVA start/end) and ASID handling hooks.
- Declare MI-consumed interfaces/macros used by `uvm_pmap.h`.
- Select TTBR/TCR settings and address size configuration macros.
- Provide phys->virt helpers for direct map if used; coordinate with `pmap_pv.h`.

3) `sys/arch/aarch64/include/pmap_pv.h`
- Define PV entry/list structures and flags for page-to-mapping tracking.
- Provide minimal interfaces/macros expected by pmap implementation.
- Ensure compatibility with UVM’s PV cache expectations.

4) `sys/arch/aarch64/include/intr.h`
- Define interrupt framework types: `intr_handle_t`, IPL levels, softints.
- Provide `spl*` macros and CPU-local pending/enable helpers.
- Declare GICv2/v3 attach hooks and `intr_establish/disestablish` prototypes.
- Document required MD initialization order (GIC before enabling IRQs).

5) `sys/arch/aarch64/include/intrdefs.h`
- Define IPL values and ordering consistent with MI `sys/sys/intr.h` users.
- Map common IPL aliases (NONE, SOFT, VM, SCHED, HIGH, etc.).
- Provide vector ranges or priorities when used by `intr.h`.

6) `sys/arch/aarch64/include/intr_distribute.h`
- Provide SMP interrupt distribution policy hooks (round-robin/affinity).
- Declare APIs for migrating/redistributing interrupts across CPUs.

7) `sys/arch/aarch64/include/pic.h`
- Define PIC interface for ARM GIC (v2/v3) integration.
- Provide PIC ops tables, CPU affinity mask types, and GIC-specific fields.
- Coordinate with `intr.h` and `intrdefs.h` for IPL/priority mapping.

8) `sys/arch/aarch64/include/armreg.h` (expand)
- Add inline accessors for key system registers (SCTLR_EL1, TCR_EL1, TTBRx_EL1,
  MAIR_EL1, TPIDR_EL0/EL1, CNT* registers, ESR/ELR/FAR).
- Define feature/ID register bitfields used by cache/MMU/probe code.
- Keep FPCR/FPSR helpers; align with `fpu.h` state layout.

9) `sys/arch/aarch64/include/fpu.h`
- Define FPU/SIMD state structure (V0–V31, FPCR, FPSR; 16-byte alignment).
- Declare save/restore helpers used by MD context switch.
- Provide flags for lazy vs eager FP state management.

10) `sys/arch/aarch64/include/elf_machdep.h`
- Set ELF machine/ABI defines (EM_AARCH64, reloc types, alignment).
- Provide pointer size/byte order, `DT_*`/`R_AARCH64_*` as required by toolchain.
- Keep MI consumers (`sys/sys/exec_elf.h`, libelf) satisfied.

11) `sys/arch/aarch64/include/cputypes.h`
- Define CPU class/feature macros relevant to AArch64 (ARMv8.x levels).
- Provide feature bitmasks queried by MI code (atomics, barriers, crypto, etc.).

12) `sys/arch/aarch64/include/cacheinfo.h`
- Define cache level descriptors and query helpers via CTR/CCSIDR registers.
- Expose line size, number of sets/ways for D/I cache and coherency hints.

13) `sys/arch/aarch64/include/machdep.h`
- Declare MD entry points: `cpu_startup`, `cpu_reboot`, `delay`, `dumpsys`.
- Provide machine limits/constants referenced by MI code.

14) `sys/arch/aarch64/include/bootinfo.h`
- Define boot information structure (FDT/ACPI pointers, memory map, initrd).
- Ensure consumers in early MD init can parse it.

15) `sys/arch/aarch64/include/loadfile_machdep.h`
- Provide loader knobs for ELF64 AArch64 (page alignment, relocations, tags).
- Coordinate with `elf_machdep.h` and bootloader expectations.

16) `sys/arch/aarch64/include/cpu_extended_state.h`
- Unify additional per-CPU state beyond FPU (e.g., SVE if ever supported).
- Provide feature-gated structures and size queries.

17) `sys/arch/aarch64/include/cpuvar.h`
- Define per-CPU structure (curpcb, CI flags, GIC CPU interface data).
- Declare `curcpu()`/`cpu_index()` helpers/macros used by MI code.

18) `sys/arch/aarch64/include/tprof.h`
- Provide PMU event/counter identifiers usable by tprof.
- Expose API surface compatible with MI profiler hooks.

19) `sys/arch/aarch64/include/pci_machdep_common.h`
- Define minimal PCI DMA/interrupt glue appropriate for AArch64 platforms.
- Document whether ACPI/MSI/MSI-X are supported and any constraints.

20) `sys/arch/aarch64/include/busdefs.h` and `bus_private.h`
- Ensure bus_space/bus_dma types and inline helpers match AArch64 ABI.
- Align with the existing `<arm/...>` bus interfaces where possible.

21) `sys/arch/aarch64/include/intr_distribute.h`, `ipmivar.h`, `genfb_machdep.h`
- Fill in only what’s required for SMP IRQ distribution, IPMI (if present), and
  generic framebuffer attach on common platforms; otherwise provide safe nops.


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
  - [ ] Provide types and helpers (`pt_entry_t`, `pde/pdp` equivalents).
  - [x] Define MAIR indices and AttrIdx helper macros.
  - [ ] Document supported page/block sizes and constraints.

- `pmap.h`:
  - [ ] Define `struct pmap` and required MI interface surface.
  - [ ] Choose VA layout and TCR config macros.
  - [ ] Add ASID helpers and per-CPU context tracking.
  - [ ] Hook PV list interactions and locking strategy.

- `pmap_pv.h`:
  - [ ] Define PV entry structs and flags.
  - [ ] Provide minimal APIs used by pmap and UVM.

- `intrdefs.h`:
  - [ ] Establish IPL levels and ordering.
  - [ ] Map MI IPL names to numeric levels.

- `intr.h`:
  - [ ] Define `intr_handle_t` and attach/establish APIs.
  - [ ] Provide `spl*` macros and softint plumbing.
  - [ ] Add GIC-specific glue and comments on init order.

- `pic.h`:
  - [ ] Define PIC ops and types suitable for GIC.
  - [ ] Add CPU affinity support for SMP.

- `armreg.h`:
  - [x] Add TCR/TTBR bitfields used by libkvm.
  - [ ] Add sysreg read/write accessors.
  - [ ] Include ID/AArch64 feature registers and masks.

- `fpu.h`:
  - [ ] Define FP/SIMD state structure and alignment.
  - [ ] Declare save/restore hooks/macros.

- `elf_machdep.h`:
  - [ ] Set EM_AARCH64 and relocation constants.
  - [ ] Verify libelf and MI ELF consumers compile.

- `cputypes.h`:
  - [ ] Define CPU feature flags and classes.
  - [ ] Provide compile-time checks for required features.

- `cacheinfo.h`:
  - [ ] Read CTR/CCSIDR/CLIDR to populate cache descriptors.
  - [ ] Expose line sizes and coherency hints.

- `machdep.h`:
  - [ ] Declare MD entry points used by MI code.
  - [ ] Provide platform constants and helper macros.

- `bootinfo.h`:
  - [ ] Define boot info struct (FDT/ACPI/memory map).
  - [ ] Align with early MD init consumers.

- `loadfile_machdep.h`:
  - [ ] Define loader page alignment and flags.
  - [ ] Ensure relocations/types match `elf_machdep.h`.

- `cpu_extended_state.h`:
  - [ ] Reserve/define additional extended state (feature-gated).

- `cpuvar.h`:
  - [ ] Define per-CPU struct and accessors.
  - [ ] Include GIC CPU interface fields if needed.

- `tprof.h`:
  - [ ] Define PMU event IDs/counters usable by tprof.
  - [ ] Provide feature checks and fallbacks.

- `pci_machdep_common.h`:
  - [ ] Minimal DMA/interrupt glue for AArch64 PCIe.
  - [ ] Decide MSI/MSI-X support policy.

- `busdefs.h` / `bus_private.h`:
  - [ ] Ensure bus_space/bus_dma types match LP64 ABI.
  - [ ] Add AArch64 shims only if generic ARM is insufficient.


Verification Steps
- [ ] Build libelf and confirm no missing AArch64 ELF macros.
- [ ] Build `lib/libkvm/kvm_aarch64.c` to validate `<machine/pte.h>`.
- [ ] Compile MI `sys/sys/intr.h` and `uvm_pmap.h` with AArch64 headers.
- [ ] Grep for includes of stubbed x86 headers; ensure no AArch64 usage.
