#!/usr/bin/env bash
set -euo pipefail

# ARM build + image wrapper for this tree
# - Builds cross toolchain and userland for ARM (32-bit) or ARM64
# - Optionally creates a bootable image using releasetools
#
# Defaults:
# - arch: arm64 (AArch64)
# - jobs: host CPU count
# - unprivileged, update build

usage() {
  cat <<USAGE
Usage: $0 [--arch arm|arm64] [--no-image] [--jobs N] [--clean]

Examples:
  # Build ARM64 toolchain + distribution + release sets + EFI image
  $0 --arch arm64

  # Build 32-bit ARM (earm) and create SD card image
  $0 --arch arm

  # Build only (no image), using 8 parallel jobs
  $0 --arch arm64 --no-image --jobs 8

Flags:
  --arch       Target architecture: arm64 (AArch64) or arm (32-bit). Default: arm64
  --no-image   Skip image generation step
  --jobs N     Parallel build jobs (defaults to CPU count)
  --clean      Clean toolchain/destdir before building (maps to build.sh -r)
  -h|--help    Show this help
USAGE
}

# Defaults
ARCH="arm64"
CREATE_IMAGE=1
CLEAN=0
JOBS=""

while [ $# -gt 0 ]; do
  case "$1" in
    --arch)
      ARCH=${2:-}
      shift 2
      ;;
    --no-image)
      CREATE_IMAGE=0
      shift
      ;;
    --jobs)
      JOBS=${2:-}
      shift 2
      ;;
    --clean)
      CLEAN=1
      shift
      ;;
    -h|--help)
      usage; exit 0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      usage; exit 1
      ;;
  esac
done

# Compute default jobs if not given
if [ -z "${JOBS}" ]; then
  JOBS=$( (sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4) )
fi

# Map arch -> MACHINE and image tool
case "${ARCH}" in
  arm64|aarch64)
    MACHINE=evbarm64-el
    IMAGE_SCRIPT="releasetools/arm64_efi_image.sh"
    ;;
  arm|earm|armv7)
    MACHINE=evbearm-el
    IMAGE_SCRIPT="releasetools/arm_sdimage.sh"
    ;;
  *)
    echo "Unsupported --arch value: ${ARCH}. Use 'arm' or 'arm64'." >&2
    exit 1
    ;;
esac

if [ ! -f build.sh ]; then
  echo "Please run from the repository root (build.sh not found)." >&2
  exit 1
fi

echo "==> Building tools for ${MACHINE} (jobs: ${JOBS})"
./build.sh -U -u ${CLEAN:+-r} -j"${JOBS}" -m "${MACHINE}" tools

echo "==> Building distribution for ${MACHINE}"
./build.sh -U -u -j"${JOBS}" -m "${MACHINE}" distribution

# When creating an image, ensure release sets exist (image scripts expect sets)
if [ ${CREATE_IMAGE} -eq 1 ]; then
  echo "==> Creating release sets for ${MACHINE}"
  ./build.sh -U -u -j"${JOBS}" -m "${MACHINE}" release

  echo "==> Generating image via ${IMAGE_SCRIPT}"
  "${IMAGE_SCRIPT}"
else
  echo "==> Skipping image generation per --no-image"
fi

echo "==> Done"

