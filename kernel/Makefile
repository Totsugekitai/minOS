CURRENT		:= $(dir $(lastword $(MAKEFILE_LIST)))

COREDIR		= $(CURRENT)core/

default:
	make -C $(COREDIR)
	cp $(COREDIR)kernel.bin .

clean:
	-rm -f kernel.bin
	make -C $(COREDIR) clean

