FLAGS=-O2 -ffunction-sections -Wl,--gc-sections,--strip-all -fno-asynchronous-unwind-tables

default: $(shell mkdir -p bin) library test

library: tinyss.cpp tinyss.hpp token.hpp
	g++ -c tinyss.cpp -o bin/tinyss.o $(FLAGS)
	ar r bin/libtinyss.a bin/tinyss.o
	rm -f bin/tinyss.o
	cp tinyss.hpp bin/tinyss.hpp
	cp token.hpp bin/token.hpp

test: bin/libtinyss.a test.cpp
	g++ test.cpp -o bin/test -Lbin -ltinyss $(FLAGS)
	
clean:
	rm -f $(wildcard bin/*)
