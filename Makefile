CC=gcc
CFLAGS=-Os -ffunction-sections -Wl,--gc-sections,--strip-all -fno-asynchronous-unwind-tables -Wall -Wextra

default: $(shell mkdir -p bin) lib test

lib: src/tinyss.h src/main.c src/var.c
	$(CC) -c src/main.c -o bin/main.o
	$(CC) -c src/var.c -o bin/var.o
	cp -f src/tinyss.h bin/
	ar r bin/libtinyss.a bin/*.o
	rm bin/*.o

test: lib src/test.c
	$(CC) src/test.c -o bin/test -Lbin -ltinyss
