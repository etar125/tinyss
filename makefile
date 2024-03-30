default: main

main:
	g++ -c tinyss.cpp -o bin/tinyss.o
	ar r bin/libtinyss.a bin/tinyss.o
	rm -rf bin/tinyss.o
	g++ test.cpp -o bin/test -Lbin -ltinyss