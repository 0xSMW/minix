# MINIX on Apple Silicon

This repository contains a MINIX 3 source tree built with the NetBSD cross-build system (`build.sh`). The focus of this fork is to build on macOS (Apple Silicon) and produce ARM64 (AArch64) EFI/GPT images suitable for virtualization. An AArch64 MINIX kernel is under active development in this tree.


## Repository Layout

- `build.sh` – Top-level NetBSD-style build driver used for cross-building the full system.
- `releasetools/` – Helpers for creating bootable images (x86 HDD/ISO, ARM SD images, etc.).
- `minix/` – MINIX kernel, servers, drivers, and userland additions.
- `share/`, `lib/`, `usr.bin/`, `usr.sbin/`, etc. – NetBSD-derived build system and userland sources.


## Requirements (macOS on Apple Silicon)

- macOS 13+ (Apple Silicon)
- Xcode Command Line Tools (provides Clang, ld, make, etc.)
  - Install: `xcode-select --install`
- Disk space: 15–25 GB for a full build (toolchain + objects + images)
- For ARM64 EFI image (optional): autotools to build GRUB (`autoconf`, `automake`, `libtool`, `pkg-config`) and network access for `git` to fetch GRUB.

No Homebrew packages are strictly required; `build.sh` bootstraps its own cross toolchain under `obj.<arch>/tooldir.*`. If you previously installed GNU tools, they won’t interfere.


## Quick Start: Build ARM64 (Apple Silicon) image

This produces an ARM64 EFI/GPT disk image with MINIX userland, an EFI System Partition, and the AArch64 kernel. Boot support is under active development.

1) Build the cross toolchain for ARM64 and the base sets:

```
./build.sh -U -u -j"$(sysctl -n hw.ncpu)" -m evbarm64-el tools
./build.sh -U -u -j"$(sysctl -n hw.ncpu)" -m evbarm64-el distribution
```

Artifacts land under:

- Tools: `obj.evbarm64-el/tooldir.Darwin-<ver>-arm64/bin`
- Destdir: `obj.evbarm64-el/destdir.evbarm64-el`
- Release sets: `obj.evbarm64-el/releasedir/evbarm64-el/binary/sets`

2) Create the ARM64 EFI disk image:

```
./releasetools/arm64_efi_image.sh
```

Output: `./minix_arm64_efi.img` (raw disk image with ESP + MINIX partitions). The script packages the built AArch64 kernel into `boot/minix_default` and writes a GRUB arm64 EFI binary and config.

One-liner make target
- From the repo root: `make apple-silicon`
  - Runs tools + distribution for `-m evbarm64-el`, then creates `minix_arm64_efi.img`.


## ARM Build Script (kernel + image)

Use the convenience wrapper to build the ARM toolchain/userland and produce an image in one go. It handles both 32‑bit ARM (earm) and ARM64 (AArch64):

Examples
- ARM64 EFI disk image: `./scripts/build-arm.sh --arch arm64`
- 32‑bit ARM SD image: `./scripts/build-arm.sh --arch arm`
- Build only (no image): `./scripts/build-arm.sh --arch arm64 --no-image`

Details
- Builds with `./build.sh -U -u -jN` and creates release sets required by the image scripts.
- For `--arch arm64`, runs `releasetools/arm64_efi_image.sh` → `minix_arm64_efi.img`.
- For `--arch arm`, runs `releasetools/arm_sdimage.sh` → `minix_arm_sd.img`.
- Optional flags: `--jobs N`, `--clean` (maps to `build.sh -r`).

Note on “ISO” vs image
- ARM generally boots from disk/SD images rather than ISO. The scripts above generate bootable disk images suited to UEFI VMs (ARM64) or SBCs/virt (ARM 32‑bit).


## Running on Apple Silicon

- Parallels Desktop (Apple Silicon): requires an ARM64 guest. Attach `minix_arm64_efi.img` as the primary disk of a new UEFI VM and boot.
- QEMU (Apple Silicon host): can boot ARM64 with UEFI firmware. Example:
  - `qemu-system-aarch64 -machine virt -cpu cortex-a57 -m 1024 -bios QEMU_EFI.fd -drive if=none,file="$(pwd)/minix_arm64_efi.img",format=raw,id=hd0 -device virtio-blk-pci,drive=hd0 -serial mon:stdio`


## Other targets

- ARM 32‑bit (earm, hardware boards):
  - Build: `./build.sh -U -u -j"$(sysctl -n hw.ncpu)" -m evbearm-el tools distribution`
  - Image: `./releasetools/arm_sdimage.sh` → `minix_arm_sd.img`
  - Run: use QEMU for ARM (emulation) or real hardware. Not runnable in Parallels on Apple Silicon.
- x86 (legacy/testing): build and image scripts remain available under `releasetools/x86_*`. Not runnable in Parallels on Apple Silicon; use QEMU/UTM or an Intel Mac.


## Customizing build output locations

You can direct `build.sh` to explicit paths for reproducible builds:

- `-D <dir>`: set `DESTDIR` (e.g., `./obj.i386/destdir.i386`)
- `-R <dir>`: set `RELEASEDIR` (e.g., `./obj.i386/releasedir/i386`)
- `-T <dir>`: set `TOOLDIR`

Example:

```
./build.sh -U -u -j"$(sysctl -n hw.ncpu)" -m evbarm64-el \
  -D "$PWD/obj.evbarm64-el/destdir.evbarm64-el" \
  -R "$PWD/obj.evbarm64-el/releasedir/evbarm64-el" \
  -T "$PWD/obj.evbarm64-el/tooldir" \
  tools distribution
```


## Useful flags and targets

- `-U`: unprivileged build (recommended on macOS)
- `-u`: update (skip clean); use `-r` to remove `TOOLDIR`/`DESTDIR` first
- `-jN`: parallelism; use `$(sysctl -n hw.ncpu)` on macOS for N
- Targets: `tools`, `distribution`, `release`

To start fresh after changing compilers or major options:

```
./build.sh -U -m evbarm64-el -r tools distribution
```


## Troubleshooting

- Missing compiler or headers on macOS: run `xcode-select --install`.
- GRUB build for arm64 fails: install autotools (`autoconf`, `automake`, `libtool`, `pkg-config`) and ensure network access for `git`.
- Permission errors writing into `obj.*`: add `-U` to build unprivileged.
- Stale toolchain after macOS/Xcode updates: rebuild with `-r`.


## License

This tree contains code under MINIX, NetBSD, and other third-party licenses. See individual file headers and `LICENSE`.
