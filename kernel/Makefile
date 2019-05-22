TARGET		= kernel.bin
CFLAGS 		= -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common
LDFLAGS		= -Map kernel.map -s -x -T kernel.ld

$(TARGET): kernel.o
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

default:
	make $(TARGET)

clean:
	-rm -f *~ *.o *.map $(TARGET)

