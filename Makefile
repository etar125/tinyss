CC=gcc
CFLAGS=-O2 -Wall
LIB=bin/libtinyss.a
TEST1=bin/test
TEST2=bin/test2
TEST3=bin/test3
LIBSRC=src/main.c src/var.c src/stack.c src/tsf.c src/tbc.c
LIBOBJ=$(patsubst src/%.c,bin/%.o,$(LIBSRC))
TEST1SRC=src/test.c
TEST1OBJ=$(patsubst src/%.c,bin/%.o,$(TEST1SRC))
TEST2SRC=src/test2.c
TEST2OBJ=$(patsubst src/%.c,bin/%.o,$(TEST2SRC))
TEST3SRC=src/test3.c
TEST3OBJ=$(patsubst src/%.c,bin/%.o,$(TEST3SRC))
.PHONY: lib tests build clean rebuild install

default: lib

all: lib tests

lib: $(LIB)

tests: $(TEST1) $(TEST2) $(TEST3)


$(LIB): $(LIBOBJ)
	ar r $@ $(LIBOBJ)

$(TEST1): $(TEST1OBJ)
	$(CC) $(TEST1OBJ) -o $@ ./bin/libtinyss.a

$(TEST2): $(TEST2OBJ)
	$(CC) $(TEST2OBJ) -o $@ ./bin/libtinyss.a
	
$(TEST3): $(TEST3OBJ)
	$(CC) $(TEST3OBJ) -o $@ ./bin/libtinyss.a

bin/%.o:
	$(CC) -Iinclude -c $(CFLAGS) $(patsubst bin/%.o,src/%.c,$@) -o $@

clean:
	rm -rf bin/*.o

rebuild: clean all

install: $(LIB)
	sudo install -m755 $(LIB) /lib/
	sudo install include/*.h /usr/include/
