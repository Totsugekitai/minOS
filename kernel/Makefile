CURRENT		:= $(dir $(lastword $(MAKEFILE_LIST)))
SCRIPT		= $(CURRENT)scripts/
INCLUDE		= $(CURRENT)include/

TARGET		= kernel.bin
CFLAGS 		= -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -O0 \
			  -I$(INCLUDE)
LDFLAGS		= -Map kernel.map -s -x -T $(SCRIPT)kernel.ld

OBJS		= core/kernel.o \
			  graphics/graphics.o \
			  init/gdt.o init/bss.o \
			  mm/segmentation.o mm/load_gdt.o mm/init_pgtable.o mm/init_cr3.o mm/change_4lv_pg.o 


$(TARGET): $(OBJS)
	ld $(LDFLAGS) -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

%.o: %.S
	as -o $@ $<

install:
	make $(TARGET)

default:
	make install

clean:
	-rm -f kernel.bin kernel.map
	make -C $(CURRENT)core/ clean
	make -C $(CURRENT)graphics/ clean
	make -C $(CURRENT)init/ clean
	make -C $(CURRENT)mm/ clean
