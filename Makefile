all: libc ts tts

libc:
	make -f Makefile.libc

ts:
	make -f Makefile.mutexts

tts:
	make -f Makefile.mutextts

compile_commands:
	bear -- make

clean:
	rm -rf target/

.PHONY = libc compile_commands clean ts tts all
