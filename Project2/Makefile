CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = fw
OBJS = fw.o
all : $(MAIN)

$(MAIN) : $(OBJS) fw.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

fw.o : fw.c fw.h fwFunc.c
	$(CC) $(CFLAGS) -c fw.c


clean :
	rm *.o $(MAIN) core
