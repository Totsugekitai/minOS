ROOTDIR			:= $(dir $(lastword $(MAKEFILE_LIST)))

TOOLS			= $(ROOTDIR)tools/
QEMU			= qemu-system-x86_64
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
	$(QEMU) -drive if=pflash,format=raw,readonly,file=$(TOOLS)OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=$(TOOLS)OVMF_VARS.fd \
		fat:rw:$(FS) -m 4G \
		-chardev stdio,mux=on,id=com1 \
		-serial chardev:com1


debug_run:
	$(QEMU) -drive if=pflash,format=raw,readonly,file=$(TOOLS)OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=$(TOOLS)OVMF_VARS.fd \
		fat:rw:$(FS) -m 4G \
		-chardev stdio,mux=on,id=com1 \
		-serial chardev:com1 \
		-monitor telnet::1234,server,nowait

clean_boot:
	-rm -r $(EDKBUILD)* $(LOADERSRC)boot
#	cd "$(ROOTDIR)kernel/" && make clean

clean_kernel:
	make -C $(KERNELSRC) clean

clean_full:
	make clean_boot && make clean_kernel

splash:
	make clean_full
	-rm -r $(FS)kernel.bin $(FS)EFI/BOOT/BOOTX64.EFI
