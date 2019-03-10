CC=g++
CFLAGS=-c -Wall -std=gnu++11

all: client clean

client: main.o tstclt.o
	$(CC) main.o tstclt.o -o ../exe/client

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

tstclt.o: tstclt.cpp
	$(CC) $(CFLAGS) tstclt.cpp

clean:
	rm -rf *.o

cleanall:
	rm -rf *.o ../exe/client
