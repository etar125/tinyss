CC := gcc
CFLAGS := -O2 -Wall -Iinclude
AR := ar
LIB := bin/libtinyss.a
TEST1 := bin/test1
TEST2 := bin/test2
TEST3 := bin/test3

LIBSRC := src/main.c src/var.c src/stack.c src/tsf.c src/tbc.c src/misc.c
TEST1SRC := src/test.c
TEST2SRC := src/test2.c
TEST3SRC := src/test3.c

LIBOBJ := $(patsubst src/%.c,bin/%.o,$(LIBSRC))
TEST1OBJ := $(patsubst src/%.c,bin/%.o,$(TEST1SRC))
TEST2OBJ := $(patsubst src/%.c,bin/%.o,$(TEST2SRC))
TEST3OBJ := $(patsubst src/%.c,bin/%.o,$(TEST3SRC))

.PHONY: all lib tests clean cleanobj rebuild install

all: lib tests

lib: $(LIB)

tests: $(TEST1) $(TEST2) $(TEST3)

$(LIB): $(LIBOBJ)
	$(AR) rcs $@ $^

$(TEST1): $(TEST1OBJ)
	$(CC) $(CFLAGS) $< -o $@ -Lbin -ltinyss

$(TEST2): $(TEST2OBJ)
	$(CC) $(CFLAGS) $< -o $@ -Lbin -ltinyss

$(TEST3): $(TEST3OBJ)
	$(CC) $(CFLAGS) $< -o $@ -Lbin -ltinyss

bin/%.o: src/%.c | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

cleanobj:
	rm -f $(LIBOBJ) $(TEST1OBJ) $(TEST2OBJ) $(TEST3OBJ)

clean: cleanobj
	rm -f $(LIB) $(TEST1) $(TEST2) $(TEST3)

rebuild: clean all

install: $(LIB)
	sudo install -d /lib /usr/include
	sudo install -m644 $(LIB) /lib/
	sudo install -m644 include/*.h /usr/include/
