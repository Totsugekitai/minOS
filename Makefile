ROOTDIR			:= $(dir $(lastword $(MAKEFILE_LIST)))

TOOLS			= $(ROOTDIR)tools/
QEMU			= $(TOOLS)qemu-system-x86_64
FS				= $(ROOTDIR)fs/
KERNELSRC		= $(ROOTDIR)kernel/
BOOTSRC			= $(ROOTDIR)boot/
EDKDIR			= $(ROOTDIR)edk2/
EDKBUILD		= $(EDKDIR)Build/
LOADERSRC		= $(EDKDIR)LoaderPkg/Applications/MinLoader/
LOADERBUILD		= $(EDKBUILD)LoaderPkgX64/NOOPT_GCC5/X64/

boot: FORCE1
	cp -r $(BOOTSRC) $(LOADERSRC)
	cd $(EDKDIR)
	build
	cp $(LOADERBUILD)MinLoader.efi $(FS)EFI/BOOT/BOOTX64.EFI

kernel: FORCE2
	make -C $(KERNELSRC)
	cp $(KERNELSRC)kernel.bin $(FS)

FORCE1:

FORCE2:

full:
	make boot
	make kernel

all:
	make full
	make run

run:
	$(QEMU) -bios $(TOOLS)OVMF.fd -pflash $(TOOLS)bios.bin \
		fat:rw:$(FS) #-monitor telnet::1234,server,nowait

debug_run:
	$(QEMU) -bios $(TOOLS)OVMF.fd -pflash $(TOOLS)bios.bin \
		fat:rw:$(FS) -S -s #-monitor telnet::1234,server,nowait

clean_boot:
	-rm -r $(EDKBUILD)* $(LOADERSRC)kernel
#	cd "$(ROOTDIR)kernel/" && make clean

clean_kernel:
	make -C $(KERNELSRC) clean

clean_full:
	make clean_boot && make clean_kernel

splash:
	make clean
	-rm -r $(FS)kernel.bin $(FS)EFI/BOOT/BOOTX64.EFI
