all: libc ts tts

libc:
	make -f Makefile.libc

ts:
	make -f Makefile.custom MUTEX=ts

tts:
	make -f Makefile.custom MUTEX=tts

compile_commands:
	bear -- make

clean:
	rm -rf target/

.PHONY = libc compile_commands clean ts tts all
