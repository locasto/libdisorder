LDFLAGS=-L../lib -L/usr/lib
INCLUDES=-I/usr/include -I../include
LIBS=-ldisorder -lm
#OPTS=-Wall -g -pg
OPTS=-Wall -g
TESTOUT=../test/testexec
TESTSRC=../test/test.c
ROPYOUT=../tools/ropy
ROPYSRC=../tools/ropy.c
TROPYOUT=../tools/tropy
TROPYSRC=../tools/tropy.c

all:	disorder library test tools

library: libdisorder.a

libdisorder.a: disorder.o
	ar rc $@ disorder.o
	mkdir -p ../lib
	mv libdisorder.a ../lib

disorder: disorder.c ../include/disorder.h
	gcc $(OPTS) -c disorder.c

test:	../test/test.c
	gcc $(OPTS) $(LDFLAGS) $(INCLUDES) -o $(TESTOUT) $(TESTSRC) $(LIBS)

tools:	ropy tropy


ropy:   ../tools/ropy.c
	gcc $(OPTS) $(LDFLAGS) $(INCLUDES) -o $(ROPYOUT) $(ROPYSRC) $(LIBS)
tropy:  ../tools/tropy.c
	gcc $(OPTS) $(LDFLAGS) $(INCLUDES) -o $(TROPYOUT) $(TROPYSRC) $(LIBS)

clean:
	@/bin/rm -f *~ *.o *.a $(TESTOUT) $(ROPYOUT) $(TROPYOUT)
	@/bin/rm -f ../test/*~
	@/bin/rm -f ../tools/*~
	@/bin/rm -Rf ../test/testexec.dSYM/
	@/bin/rm -Rf ../tools/ropy.dSYM/
	@/bin/rm -Rf ../tools/tropy.dSYM/
	@/bin/rm -f ../include/*~
	@/bin/rm -f ../lib/*.a
	@/bin/rm -f ../*~
