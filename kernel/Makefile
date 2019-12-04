CURRENT		:= $(dir $(lastword $(MAKEFILE_LIST)))
SCRIPT		= $(CURRENT)scripts/
INCLUDE		= $(CURRENT)include/

TARGET		= kernel.bin
CFLAGS 		= -g -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common \
			  -O0 -I /usr/include -I $(INCLUDE) -fno-stack-protector -fno-pic \
			  -mno-red-zone -Wno-unused-parameter -mno-avx -mgeneral-regs-only
			  
LDFLAGS		= -Map kernel.map -s -x -T $(SCRIPT)kernel.ld

OBJS		= core/kernel.o \
			  graphics/graphics.o \
			  util/util.o \
			  util/util_asm.o \
			  init/init_settings.o \
			  mm/segmentation.o \
			  mm/paging.o \
			  mm/memory.o \
			  mm/memory_asm.o \
			  debug/debug.o \
			  debug/debug_asm.o \
			  interrupt/load_idt.o \
			  interrupt/interrupt.o \
			  interrupt/fault_handlers.o \
			  interrupt/user_defined_handlers.o \
			  device/keyboard.o \
			  device/keyinput.o \
			  device/pci.o \
			  app/console.o \
			  acpi/acpi.o \
			  command/echo.o \
			  command/uptime.o \
			  command/sleep.o \
			  task/thread.o \
			  task/thread_asm.o \

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
	make -C $(CURRENT)command/ clean
	make -C $(CURRENT)task/ clean
