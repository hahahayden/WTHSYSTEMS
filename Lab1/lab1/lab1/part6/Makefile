CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = part6_tests
OBJS = part6_tests.o part6.o
all : $(MAIN)

$(MAIN) : $(OBJS) part6.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

part6_tests.o : part6_tests.c part6.h
	$(CC) $(CFLAGS) -c part6_tests.c

part6.o : part6.c part6.h
	$(CC) $(CFLAGS) -c part6.c

clean :
	rm *.o $(MAIN) core
