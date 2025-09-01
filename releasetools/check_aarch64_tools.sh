#!/usr/bin/env bash
set -euo pipefail

# Verify presence of key cross tools for AArch64 on a macOS arm64 host.

ARCH=${ARCH:-evbarm64-el}
OBJ=${OBJ:-../obj.${ARCH}}
TOOLDIR=${TOOLDIR:-${OBJ}/"tooldir.$(uname -s)-$(uname -r)-$(uname -m)"}

# Default to Minix AArch64 triple
MACHINE_GNU_PLATFORM=${MACHINE_GNU_PLATFORM:-aarch64-elf64-minix}

TOOLS=(
  "${MACHINE_GNU_PLATFORM}-ar"
  "${MACHINE_GNU_PLATFORM}-as"
  "${MACHINE_GNU_PLATFORM}-ld"
  "${MACHINE_GNU_PLATFORM}-nm"
  "${MACHINE_GNU_PLATFORM}-objcopy"
  "${MACHINE_GNU_PLATFORM}-objdump"
  "${MACHINE_GNU_PLATFORM}-ranlib"
  "${MACHINE_GNU_PLATFORM}-readelf"
  "${MACHINE_GNU_PLATFORM}-size"
  "${MACHINE_GNU_PLATFORM}-strings"
  "${MACHINE_GNU_PLATFORM}-strip"
  "${MACHINE_GNU_PLATFORM}-clang"
  "${MACHINE_GNU_PLATFORM}-clang++"
)

BIN="${TOOLDIR}/bin"

echo "Checking tool presence in: ${BIN}"
missing=0
for t in "${TOOLS[@]}"; do
  if [ ! -x "${BIN}/${t}" ]; then
    echo "MISSING: ${BIN}/${t}"
    missing=1
  else
    echo "OK: ${t}"
  fi
done

echo ""
if [ ${missing} -eq 0 ]; then
  echo "All expected AArch64 cross tools present."
else
  echo "Some tools are missing. Run 'make apple-silicon' tools stage, or clean tooldir and rebuild."
  exit 1
fi

