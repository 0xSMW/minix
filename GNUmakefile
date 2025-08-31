## Convenience wrapper for GNU make users on macOS
## Redirects `make apple-silicon` to the same steps defined in the BSD Makefile.

.PHONY: apple-silicon
apple-silicon:
	@echo "[apple-silicon] Building evbarm64 tools..."
	PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin ./build.sh -U -u -j$(shell sysctl -n hw.ncpu 2>/dev/null || echo 4) -m evbarm64-el -V MKINFO=no tools
	@echo "[apple-silicon] Building evbarm64 distribution..."
	PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin ./build.sh -U -u -j$(shell sysctl -n hw.ncpu 2>/dev/null || echo 4) -m evbarm64-el -V MKINFO=no distribution
	@echo "[apple-silicon] Creating ARM64 EFI image..."
	bash ./releasetools/arm64_efi_image.sh
