#!/usr/bin/env bash
set -e

# ARM64 (AArch64) EFI/GPT image builder for Apple Silicon virtualization
#
# This script creates a bootable-style disk image layout for arm64. It relies on
# the NetBSD-style cross tools (nb*) produced by build.sh. It expects an
# AArch64 MINIX kernel and modules in the minix-kernel.tgz set; if not found,
# it will emit a warning and produce an image with an ESP and filesystems but
# without a bootable kernel (for early pipeline bring-up).

: ${ARCH=evbarm64-el}
: ${OBJ=../obj.${ARCH}}
: ${TOOLCHAIN_TRIPLET=aarch64-elf32-minix-}
: ${BUILDSH=build.sh}

# Image contents
: ${SETS="minix-base minix-comp minix-games minix-man minix-tests tests"}
: ${IMG=minix_arm64_efi.img}
:
# Do not try to (re)build the entire Minix source tree from this script.
# We only assemble an image scaffold from whatever sets/kernels are present.
CREATE_IMAGE_ONLY=1

# EFI partition size (bytes)
: ${EFI_SIZE=$(( 64*(2**20) ))}

# Disk partition sizes (bytes); choose 2 GiB total footprint by default
: ${ROOT_SIZE=$((  128*(2**20) ))}
: ${HOME_SIZE=$((  128*(2**20) ))}
: ${USR_SIZE=$((  1792*(2**20) ))}

if [ ! -f ${BUILDSH} ]
then
    echo "Please invoke me from the root source dir, where ${BUILDSH} is."
    exit 1
fi

# Environment setup
. releasetools/image.defaults

# When creating an image scaffold only, tolerate missing DESTDIR/RELEASEDIR
if [ ${CREATE_IMAGE_ONLY} -eq 1 ]
then
    mkdir -p "${DESTDIR}" "${RELEASEDIR}" || true
fi

. releasetools/image.functions

echo "Building work directory..."
build_workdir "$SETS"

echo "Adding kernel files (if available)..."
KERNEL_SET="${SETS_DIR}/minix-kernel.tgz"
if [ -e "${KERNEL_SET}" ]; then
    add_link_spec "boot/minix_latest" "minix_default" extra.kernel
    workdir_add_kernel minix_default
else
    echo "Warning: ${KERNEL_SET} not found (no AArch64 kernel in this tree)."
    echo "Proceeding to build filesystems and ESP without kernel."
    mkdir -p ${ROOT_DIR}/boot/minix_default
    add_dir_spec "boot/minix_default" extra.kernel
    echo "AArch64 MINIX kernel not present in this source tree." \
      > ${ROOT_DIR}/boot/minix_default/README.txt
    add_file_spec "boot/minix_default/README.txt" extra.kernel
fi

echo "Bundling packages (if any)..."
bundle_packages "$BUNDLE_PACKAGES"

echo "Creating specification files..."
create_input_spec
create_protos "usr home"

# Clean previous image
if [ -f ${IMG} ]
then
    rm -f ${IMG}
fi

echo "Writing disk image..."

# All sizes are written in 512-byte blocks
ROOTSIZEARG="-b $((${ROOT_SIZE} / 512 / 8))"
USRSIZEARG="-b $((${USR_SIZE} / 512 / 8))"
HOMESIZEARG="-b $((${HOME_SIZE} / 512 / 8))"

# Simple alignment: reserve initial sectors for alignment (1 MiB)
: ${BOOT_SECTORS=2048}

# Lay out MINIX partitions directly into the IMG
ROOT_START=${BOOT_SECTORS}
echo " * ROOT"
_ROOT_SIZE=$(${CROSS_TOOLS}/nbmkfs.mfs -d ${ROOTSIZEARG} -I $((${ROOT_START}*512)) ${IMG} ${WORK_DIR}/proto.root)
_ROOT_SIZE=$(($_ROOT_SIZE / 512))
USR_START=$((${ROOT_START} + ${_ROOT_SIZE}))
echo " * USR"
_USR_SIZE=$(${CROSS_TOOLS}/nbmkfs.mfs  -d ${USRSIZEARG}  -I $((${USR_START}*512))  ${IMG} ${WORK_DIR}/proto.usr)
_USR_SIZE=$(($_USR_SIZE / 512))
HOME_START=$((${USR_START} + ${_USR_SIZE}))
echo " * HOME"
_HOME_SIZE=$(${CROSS_TOOLS}/nbmkfs.mfs -d ${HOMESIZEARG} -I $((${HOME_START}*512)) ${IMG} ${WORK_DIR}/proto.home)
_HOME_SIZE=$(($_HOME_SIZE / 512))

# Build EFI system partition with GRUB (arm64)
echo " * EFI"
fetch_and_build_grub_arm64 || echo "Warning: failed to build GRUB arm64 EFI binary; ensure autotools deps are installed."

: ${EFI_DIR=$OBJ/efi_arm64}
rm -rf ${EFI_DIR} && mkdir -p ${EFI_DIR}/boot/minix_default ${EFI_DIR}/boot/efi

# Populate boot files onto the ESP
if [ -d ${MODDIR} ]; then
  cp ${MODDIR}/* ${EFI_DIR}/boot/minix_default/ || true
fi

# Copy GRUB EFI binary if present
if [ -f ${RELEASETOOLSDIR}/grub/grub-core/bootaa64.efi ]; then
  cp ${RELEASETOOLSDIR}/grub/grub-core/bootaa64.efi ${EFI_DIR}/boot/efi/BOOTAA64.EFI
  create_grub_cfg_arm64
else
  echo "GRUB arm64 EFI binary not found; placing placeholder." > ${EFI_DIR}/boot/efi/README.txt
fi

# Create FAT32 ESP image, append to end of disk image, and set partition table
dd if=/dev/zero bs=${EFI_SIZE} count=1 > ${OBJ}/efi_arm64.img
${CROSS_TOOLS}/nbmakefs -t msdos -s ${EFI_SIZE} -o "F=32,c=1" ${OBJ}/efi_arm64.img ${EFI_DIR}
dd if=${OBJ}/efi_arm64.img >> ${IMG}

# Partition map (MBR with an EFI partition entry + MINIX partitions)
${CROSS_TOOLS}/nbpartition -m ${IMG} ${BOOT_SECTORS} 81:${_ROOT_SIZE}* 81:${_USR_SIZE} 81:${_HOME_SIZE} EF:1+

echo ""
echo "ARM64 EFI disk image at $(pwd)/${IMG}"
echo ""
echo "To try under QEMU (Apple Silicon host):"
echo "qemu-system-aarch64 -machine virt -cpu cortex-a57 -m 1024 \\
      -bios QEMU_EFI.fd -drive if=none,file=$(pwd)/${IMG},format=raw,id=hd0 \\
      -device virtio-blk-pci,drive=hd0 -serial mon:stdio"
echo ""
echo "Parallels (Apple Silicon): create a new VM with a blank disk, then replace its disk with $(pwd)/${IMG}. Ensure UEFI boot is enabled."
