.PHONY:clean

all:lib/main.o lib/test.o
	gcc lib/main.o lib/test.o -o bin/test

lib/main.o:include/test.h src/main.c
	gcc -c -I ./include/ src/main.c -o lib/main.o

lib/test.o:include/test.h src/test.c
	gcc -c -I./include src/test.c -o lib/test.o

clean:
	rm -rf lib/test.o lib/main.o bin/test
