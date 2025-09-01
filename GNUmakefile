.PHONY: arm arm64 apple-silicon

# GNUmakefile provides convenience targets for environments where `make`
# is GNU Make and cannot parse the BSD-style top-level Makefile.

arm64:
	@echo "[gnu] invoking scripts/build-arm.sh --arch arm64"
	bash ./scripts/build-arm.sh --arch arm64

arm:
	@echo "[gnu] invoking scripts/build-arm.sh --arch arm"
	bash ./scripts/build-arm.sh --arch arm

# Keep legacy convenience target name from README
apple-silicon: arm64

