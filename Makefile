CC = gcc
CFLAGS = -Wall -g

all: main



main.o: main.c types.h
    $(CC) $(CFLAGS) -c main.c

main: main.o mkdir_splitpath.o
    $(CC) $(CFLAGS) -o main main.o mkdir_splitpath.o

mkdir_splitpath.o: mkdir_splitpath.c types.h
    $(CC) $(CFLAGS) -c mkdir_splitpath.c

clean:
    rm -f *.o main