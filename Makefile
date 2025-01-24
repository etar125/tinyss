CC=gcc
CFLAGS=-Os -ffunction-sections -Wl,--gc-sections,--strip-all -fno-asynchronous-unwind-tables -Wall -Wextra

build: $(shell mkdir -p bin) lib test

lib: src/tinyss.h src/main.c src/var.c
	$(CC) -c src/main.c -o bin/main.o $(CFLAGS)
	$(CC) -c src/var.c -o bin/var.o $(CFLAGS)
	cp -f src/tinyss.h bin/
	ar r bin/libtinyss.a bin/*.o
	rm bin/*.o

test: lib src/test.c
	$(CC) src/test.c -o bin/test -Lbin -ltinyss $(CFLAGS)

clean:
	rm -rf bin/*

rebuild: clean build
