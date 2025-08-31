% AArch64 (Apple Silicon) Migration Plan for MINIX

This plan tracks the work required to bring up a functional AArch64 (arm64) port of MINIX that builds and runs under UEFI on Apple Silicon and QEMU’s `virt` machine. It’s organized in phases with concrete tasks and acceptance criteria.

## Phase 0 — Orientation & Scope

- [ ] Define ABI and target triple
  - Choose LP64 AArch64 ELF ABI (little‑endian), use `aarch64-elf64-minix` as the canonical target triple.
  - Document register conventions, syscall calling convention, stack alignment, TLS model, and relocation model.
- [ ] Identify boot strategy under UEFI
  - Decide between: GRUB arm64 with a Multiboot‑compatible loader vs. a custom EFI loader that loads the MINIX ELF kernel and modules.
  - Acceptance: one chosen path with a minimal loader spec.

## Phase 1 — Build System & Toolchain

- [ ] Update build system target triple
  - share/mk/bsd.own.mk: define `MACHINE_GNU_PLATFORM` for AArch64 as `aarch64-elf64-minix` (and keep `MACHINE_GNU_ARCH` detection intact).
- [ ] Binutils support for the triple
  - tools/binutils: configure BFD/ld for `aarch64-elf64-minix` (temporary alias to `aarch64-elf` acceptable initially).
  - Provide ld emulation (`eaarch64minix*`) or reuse generic AArch64 ELF emulation.
  - Acceptance: `aarch64-elf64-minix-objcopy`, `-ld`, `-nm`, `-strip` build and run.
- [ ] Compiler (GCC/Clang) support
  - Ensure Clang/LLVM builds target `aarch64-elf64-minix` (external/bsd/llvm config default triple and targets include AArch64).
  - Ensure libgcc/compiler_rt provide required builtins for AArch64 (use compiler_rt; repo already contains aarch64 compiler_rt areas).
  - Acceptance: trivial hello‑world for AArch64 cross‑compiles and links against MINIX libc/sysroot.
- [ ] Host tools parity on macOS
  - Confirm host tools (nbcompat, mkfs.mfs, nbpartition, etc.) build cleanly on macOS arm64.

## Phase 2 — Arch Headers & libc glue

- [ ] Create `minix/include/arch/aarch64/include/`
  - Initial files: `archconst.h`, `archtypes.h`, `elf.h`, `ipcconst.h`, `memory.h`, `partition.h`, `stackframe.h`, `vm.h`.
  - Derive from `earm` and adjust for AArch64 LP64 sizes and ABI.
- [ ] libc arch shims
  - Verify `lib/libc/arch/aarch64/*` from NetBSD are integrated into the MINIX build where applicable.
  - Wire up signal trampolines, setjmp/longjmp, errno/PSR access if needed.
  - Acceptance: libc for AArch64 compiles for the MINIX sysroot.
- [ ] Remove temporary header workarounds
  - Delete the i386 fallback include from `minix/commands/partition/Makefile` once `arch/aarch64` exists.

## Phase 3 — libsys (system call ABI)

- [ ] Implement syscall stubs/trampolines for AArch64
  - Add `syscall`/SVC entry glue; parameter passing via x0‑x7; error returns in x0 with carry semantics as needed.
  - Context save/restore helpers, userland entry sequences, thread‑local storage setup.
  - Acceptance: basic syscalls link and a trivial program invoking `_exit(0)` runs under QEMU when the kernel lands.

## Phase 4 — Kernel Bring‑up (arch/aarch64)

- [ ] Directory and core files
  - `minix/kernel/arch/aarch64/`: `head.S`, exception vector table, `exception.c`, `klib.S`, `mpx.S` analogs, `procoffsets.cf`, `kernel.lds` for AArch64.
- [ ] MMU & paging
  - Early page tables (TTBRs), memory map, identity map for early boot, switch to kernel VA layout.
- [ ] Interrupts & timer
  - GICv2/v3 support (start with GICv2 on QEMU virt), generic ARM timer (cntvct/cnthp), IRQ routing.
- [ ] Context switching & traps
  - User/kernel mode transitions, signal delivery/return, copyin/copyout.
- [ ] SMP (optional later)
  - PSCI bring‑up for secondary cores; initial target can be UP.
  - Acceptance: kernel enters main, prints banner on PL011, handles timer interrupts, and idles.

## Phase 5 — Platform/Board Support

- [ ] QEMU `virt` machine as the reference platform
  - Drivers: PL011 UART for console, virtio‑blk, virtio‑net (later), generic RTC if needed.
  - PSCI for power management calls.
- [ ] Apple Silicon Parallels VM profile
  - Validate UEFI boot, expose virtio devices similarly; confirm console path.
  - Acceptance: serial console output visible; virtio‑blk used for rootfs.

## Phase 6 — Servers & Drivers Audit

- [ ] Audit servers for 64‑bit assumptions
  - `rs`, `pm`, `vfs`, `vm`, `ds`, `sched`, `mib`, etc. Fix size_t/ptrdiff_t and pointer truncation issues.
- [ ] Drivers
  - TTY via PL011; block via virtio‑blk; network via virtio‑net (post‑MVP).
  - Acceptance: minimal single‑user boot to shell on a RAM disk or virtio disk.

## Phase 7 — Boot Loader Path (UEFI)

- [ ] Decide and implement loader
  - Option A: GRUB arm64 with a MINIX loader module (Multiboot‑like handoff) — requires writing a loader that prepares bootinfo and loads modules.
  - Option B: Custom EFI app that locates and loads the MINIX ELF kernel and modules and sets up a bootinfo structure.
- [ ] Image builder adaptations
  - Update `releasetools/arm64_efi_image.sh` and `image.functions` to write the correct loader and config (GRUB cfg or start.efi) for AArch64.
  - Acceptance: `minix_arm64_efi.img` boots to the MINIX kernel on QEMU/Parallels.

## Phase 8 — Distribution & Sets

- [ ] Remove the `MKMINIX=no` gating for AArch64
  - Enable the Minix subtree for `evbarm64*` once kernel+userland build.
- [ ] Sets composition
  - Add AArch64 to sets lists; ensure `minix-base`, `minix-comp`, `minix-man`, etc., include AArch64 outputs.
- [ ] Release tools
  - Confirm `releasetools/` scripts handle AArch64 (paths, partitions, sizes). Adjust `image.defaults` for arm64 naming and sizes.
  - Acceptance: `./build.sh -m evbarm64-el distribution` produces complete sets including kernel (`minix-kernel.tgz`).

## Phase 9 — Testing & CI

- [ ] QEMU smoke tests
  - Boot image non‑interactively to a shell; verify simple commands; run a sanity test suite.
- [ ] Parallels validation
  - Produce a VM template; verify console, disk, and network (virtio/net if available).
- [ ] Automated builds on macOS (arm64)
  - Scripted toolchain + userland + image build; retain logs/artifacts.

## Phase 10 — Cleanup & Hardening

- [ ] Remove temporary workarounds
  - Revert i386 partition header fallbacks; drop zero‑decls in `installboot` once those arches are included or conditionally compiled.
- [ ] Documentation
  - Update README with AArch64 build/run instructions; add troubleshooting.
- [ ] Performance & robustness
  - Enable SMP; tune timers; review memory map; audit cache/TLB barriers in context switches and drivers.

## Current Repo Deltas (Already Applied)

- Binutils: target aliasing for AArch64; forced reconfigure when changed.
- Host fixes on macOS: compat headers, texinfo includes.
- Installboot linking: declared missing `ib_mach_*` to unblock host tool.
- Partition host include fallback for aarch64 (temporary).
- `apple-silicon` target and ARM64 EFI image builder (scaffold mode).
- Makefile gating to optionally skip Minix subtree on AArch64 for NetBSD userland reference builds.

## Acceptance Milestones

1. Cross‑tools + NetBSD userland build for `evbarm64-el` (reference only).
2. MINIX libc+libsys compile and link for AArch64.
3. Kernel boots to single user on QEMU `virt` with PL011 console.
4. Boot from `minix_arm64_efi.img` under UEFI (GRUB/custom loader) with modules.
5. Multi‑user bring‑up with storage (virtio‑blk) and TTY.
6. Parallels validation; docs complete; nightly CI green.

## Notes & Constraints

- We will keep using Clang/LLD where possible on macOS; binutils/BFD are built for the cross target only.
- If GRUB path becomes burdensome on arm64, pivot to a tiny custom EFI loader for MINIX.
- Keep changes minimal and gated to AArch64 until the port matures; do not regress i386/earm.

