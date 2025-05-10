CC=gcc
CFLAGS=-O2 -Wall
LIB=bin/libtinyss.a
TEST=bin/test
LIBSRC=src/main.c src/var.c src/stack.c
LIBOBJ=$(patsubst src/%.c,bin/%.o,$(LIBSRC))
TESTSRC=src/test.c
TESTOBJ=$(patsubst src/%.c,bin/%.o,$(TESTSRC))
.PHONY: lib test build clean rebuild install

lib: $(LIB)

test: $(TEST)

build: $(shell mkdir -p bin) $(LIB) $(TEST)

$(LIB): $(LIBOBJ)
	ar r $@ $(LIBOBJ)
	cp src/tinyss.h bin/

$(TEST): $(TESTOBJ)
	$(CC) $(TESTOBJ) -o $@ -Lbin -ltinyss 

bin/%.o:
	$(CC) -c $(CFLAGS) $(patsubst bin/%.o,src/%.c,$@) -o $@

clean:
	rm -rf bin/*.o

rebuild: clean build

install: $(LIB)
	install -m755 $(LIB) /lib/
	install include/*.h /usr/include/
