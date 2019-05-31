CURRENT		:= $(dir $(lastword $(MAKEFILE_LIST)))
INCLUDE		= $(CURRENT)../include/

TARGET		= kernel.o
CFLAGS 		= -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -O0 \
			  -I $(INCLUDE)

CFILES		= kernel.c

$(TARGET): $(CFILES)
	gcc $(CFLAGS) -c -o $@ $(CFILES)

default:
	make $(TARGET)

clean:
	-rm -f *.o

