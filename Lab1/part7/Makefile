CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = commandLineCount
OBJS = commandLineCount.o
all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)


#commandLineCount.o: commandLineCount.c
#	$(CC) $(CFLAGS) -c commandLineCount.c

clean :
	rm *.o $(MAIN) core
