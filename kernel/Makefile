TARGET = kernel.bin
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common \
		 -I /usr/include/efi
LDFLAGS = -Map kernel.map -s -x -T kernel.ld

$(TARGET): kernel.o
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	cp $(TARGET) ../fs/
	qemu-system-x86_64 -m 4G -bios ../tools/OVMF.fd -hda fat:rw:../fs

clean:
	rm -f *~ *.o *.map $(TARGET)


