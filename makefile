default: main

main: library test

library:
	clang++ -c tinyss.cpp -o bin/tinyss.o
	ar r bin/libtinyss.a bin/tinyss.o
	rm -rf bin/tinyss.o
	cp tinyss.hpp bin/tinyss.hpp
	cp token.hpp bin/token.hpp

test:
	clang++ test.cpp -o bin/test -Lbin -ltinyss