CC=gcc
CFLAGS= -Wall -g -pedantic -std=c11
LDLIBS= -lm

ALL = test_unbounded calc_unbounded_int

all: $(ALL)

test_unbounded: test_unbounded.o unbounded_int.o
	$(CC) -o test_unbounded test_unbounded.o unbounded_int.o $(LDLIBS)

calc_unbounded_int: calc_unbounded_int.o unbounded_int.o
	$(CC) -o calc_unbounded_int calc_unbounded_int.o unbounded_int.o $(LDLIBS)

test_unbounded.o : test_unbounded.c
	$(CC) $(CFLAGS) -c -o test_unbounded.o test_unbounded.c

calc_unbounded_int.o : calc_unbounded_int.c
	$(CC) $(CFLAGS) -c -o calc_unbounded_int.o calc_unbounded_int.c

unbounded_int.o: unbounded_int.c unbounded_int.h
	$(CC) -c $(CFLAGS) -o unbounded_int.o unbounded_int.c

clean:
	rm -rf *~ *.o $(ALL) *.stackdump