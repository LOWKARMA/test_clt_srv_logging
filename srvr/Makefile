CC = g++
CFLAGS = -c -Wall -std=gnu++11 -pipe
LIBS = -lpthread 

all: server clean

server: main.o testsrv.o tsetlog.o
	$(CC) main.o testsrv.o tsetlog.o $(LIBS) -o ../exe/server

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

testsrv.o: testsrv.cpp
	$(CC) $(CFLAGS) testsrv.cpp 

tsetlog.o: tsetlog.cpp
	$(CC) $(CFLAGS) tsetlog.cpp 

clean:
	rm -rf *.o

cleanall:
	rm -rf *.o ../exe/server
