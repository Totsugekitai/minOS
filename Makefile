ROOTDIR		:= $(dir $(lastword $(MAKEFILE_LIST)))
TOOLS		= $(ROOTDIR)tools/
FS			= $(ROOTDIR)fs/
EDKBUILD	= $(ROOTDIR)edk2/Build/
MIN_LOADER	= $(EDKBUILD)LoaderPkgX64/NOOPT_GCC5/X64/

loader:
	cd edk2
	-source ./edksetup.sh
	build
	cd ../
	cp $(MIN_LOADER)MinLoader.efi $(FS)EFI/BOOT/BOOTX64.EFI

install:
	make loader

run:
	make install
	qemu-system-x86_64 -bios $(TOOLS)OVMF.fd -pflash $(TOOLS)bios.bin \
		fat:rw:$(FS) -monitor telnet::1234,server,nowait
