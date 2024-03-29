default: main

main:
	g++ -c main.cpp
	ar r tinyss.a main.o
	rm -rf main.o