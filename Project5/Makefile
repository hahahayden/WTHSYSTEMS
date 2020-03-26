CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -std=gnu99
MAIN = parseline
OBJS = parseline.o


$(MAIN) : $(OBJS) 
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

parseline.o : parseline.c
	$(CC) $(CFLAGS) -c parseline.c

clean :
	rm *.o $(MAIN) core*