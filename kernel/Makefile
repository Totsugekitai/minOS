CURRENT		:= $(dir $(lastword $(MAKEFILE_LIST)))
SCRIPT		= $(CURRENT)scripts/
INCLUDE		= $(CURRENT)include/

TARGET		= kernel.bin
CFLAGS 		= -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -O0 \
			  -I$(INCLUDE) #-T $(SCRIPT)kernel.ld -Xlinker -Map=kernel.map
LDFLAGS		= -Map kernel.map -s -x -T $(SCRIPT)kernel.ld

CFILES		= kernel.c disp_square.c
OBJFILES	= kernel.o disp_square.o

$(TARGET): $(OBJFILES)
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

%.c:
	cp $(CURRENT)*/*.c $(CURRENT)

install:
	make $(TARGET)
	rm -f $(CFILES) $(OBJFILES)

default:
	make install

clean:
	-rm -f kernel.bin *.o *.c *.map
	make -C $(CURRENT)core/ clean
	make -C $(CURRENT)graphics/ clean

