CURRENT		:= $(dir $(lastword $(MAKEFILE_LIST)))
SCRIPT		= $(CURRENT)scripts/
INCLUDE		= $(CURRENT)include/

TARGET		= kernel.bin
CFLAGS 		= -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -O0 \
			  -I /usr/include -I $(INCLUDE) -fno-stack-protector -fno-pic
LDFLAGS		= -Map kernel.map -s -x -T $(SCRIPT)kernel.ld

OBJS		= core/kernel.o \
			  graphics/graphics.o \
			  util/util.o \
			  util/util_asm.o \
			  init/bss.o \
			  mm/segmentation.o \
			  mm/load_gdt.o \
			  mm/set_segment_register.o \
			  mm/load_pgtable.o \
			  mm/paging.o \
			  debug/debug.o \
			  interrupt/load_idt.o \
			  interrupt/interrupt.o \
			  interrupt/fault_handler.o \
			  interrupt/handler_caller.o \
			  device/keyboard.o \
			  device/keyinput.o \
			  app/console.o \
			#   acpi/acpi.o \
			#   acpi/acpi_asm.o

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
	-rm -f kernel.bin
	make -C $(CURRENT)core/ clean
	make -C $(CURRENT)graphics/ clean
	make -C $(CURRENT)init/ clean
	make -C $(CURRENT)mm/ clean
	make -C $(CURRENT)debug/ clean
	make -C $(CURRENT)util/ clean
	make -C $(CURRENT)interrupt/ clean
	make -C $(CURRENT)device/ clean
	make -C $(CURRENT)app/ clean
	make -C $(CURRENT)acpi/ clean

