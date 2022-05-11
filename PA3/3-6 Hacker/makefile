all: crc32.o main.o
	g++ crc32.o main.o -o main

crc32.o: crc32.cpp crc32.h
	g++ -c crc32.cpp -o crc32.o

main.o: main.cpp crc32.cpp crc32.h
	g++ -c main.cpp -o main.o

.PHONY: clean
clean:
	rm *.o main
