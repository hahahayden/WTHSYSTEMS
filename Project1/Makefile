CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = mytr
OBJS = mytr.o
all : $(MAIN)

$(MAIN) : $(OBJS) mytr.h 
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mytr.o : mytr.c mytr.h
	$(CC) $(CFLAGS) -c mytr.c


clean :
	rm *.o $(MAIN) core
