CC = gcc
CFLAGS = -Wall -g
OBJECTS = main.o mkdir_splitpath.o other_commands.o

# This is the rule to build the final executable
main: $(OBJECTS)
    $(CC) $(CFLAGS) -o main $(OBJECTS)

# Rule for compiling the main.c file
main.o: main.c types.h
    $(CC) $(CFLAGS) -c main.c

# Rule for compiling the mkdir_splitpath.c file
mkdir_splitpath.o: mkdir_splitpath.c types.h
    $(CC) $(CFLAGS) -c mkdir_splitpath.c

# Clean rule to remove object files and the executable
clean:
    rm -f *.o main