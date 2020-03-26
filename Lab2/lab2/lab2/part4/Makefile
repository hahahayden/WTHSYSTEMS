CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = example
OBJS = example.o fact.o
all : $(MAIN)

$(MAIN) : $(OBJS) fact.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

example.o : example.c fact.h
	$(CC) $(CFLAGS) -c example.c -g

fact.o : fact.c fact.h
	$(CC) $(CFLAGS) -c fact.c -g

clean :
	rm *.o $(MAIN) core
