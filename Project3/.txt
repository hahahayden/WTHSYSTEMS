CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -std=gnu99
MAIN = hencode hdecode
OBJS = hencode.o hdecode.o
all : hencode.o hdecode.o
		gcc -o hencode -Wall -ansi -pedantic -g -std=gnu99 hencode.c
		gcc -o hdecode -Wall -ansi -pedantic -g -std=gnu99 hdecode.c

$(MAIN) : $(OBJS) hencode.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

hencode.o : hencode.c hencode.h
	$(CC) $(CFLAGS) -c hencode.c
hdecode.o : hdecode.c hencode.h
	$(CC) $(CFLAGS) -c hdecode.c
clean :
	rm *.o $(MAIN) core