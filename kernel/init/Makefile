CURRENT		:= $(dir $(lastword $(MAKEFILE_LIST)))
INCLUDE		= $(CURRENT)../include/

TARGET		= init.o
CFLAGS 		= -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -O0 \
			  -I $(INCLUDE)

CFILES		= init.c bss.c

$(TARGET): $(CFILES)
	gcc $(CFLAGS) -c -o $@ $(CFILES)

default:
	make $(TARGET)

clean:
	-rm -f *.o

