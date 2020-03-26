CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -std=gnu99
MAIN = mytar
OBJS = mytar.o

$(MAIN) : $(OBJS) 
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mytar.o : mytar.c mytar.h
	$(CC) $(CFLAGS) -c mytar.c

clean :
	rm *.o $(MAIN) core