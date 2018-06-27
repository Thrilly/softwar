CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=

all: pub

pub: pub_server.o
	gcc -o server pub_server.o

pub_server.o: pub_server.c
	gcc -o pub_server.o -c pub_server.c $(LDFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello
