ROOTDIR		:= $(dir $(lastword $(MAKEFILE_LIST)))

TOOLS			= $(ROOTDIR)tools/
QEMU			= $(TOOLS)qemu-system-x86_64
FS				= $(ROOTDIR)fs/
EDKDIR			= $(ROOTDIR)edk2/
EDKBUILD		= $(EDKDIR)Build/
LOADERSRC		= $(EDKDIR)LoaderPkg/Applications/MinLoader/
LOADERBUILD		= $(EDKBUILD)LoaderPkgX64/NOOPT_GCC5/X64/

loader:
	cp -r kernel $(LOADERSRC)
	cd $(EDKDIR)
	-source edksetup.sh
	build
	cp $(LOADERBUILD)MinLoader.efi $(FS)EFI/BOOT/BOOTX64.EFI

install:
	make loader

run:
	make install
	$(QEMU) -bios $(TOOLS)OVMF.fd -pflash $(TOOLS)bios.bin \
		fat:rw:$(FS) -monitor telnet::1234,server,nowait

clean:
	-rm -r $(EDKBUILD)* $(FS)EFI/BOOT/BOOTX64.EFI $(LOADERSRC)kernel/
