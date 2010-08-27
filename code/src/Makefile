LDFLAGS=-L../lib -L/usr/lib
INCLUDES=-I/usr/include -I../include
LIBS=-ldisorder -lm
#OPTS=-Wall -g -pg
OPTS=-Wall -g
TESTOUT=../test/testexec
TESTSRC=../test/test.c
TOOLOUT=../tool/ropy
TOOLSRC=../tool/ropy.c

all:	disorder library test tool

library: libdisorder.a

libdisorder.a: disorder.o
	ar rc $@ disorder.o
	mkdir -p ../lib
	mv libdisorder.a ../lib

disorder: disorder.c ../include/disorder.h
	gcc $(OPTS) -c disorder.c

test:	../test/test.c
	gcc $(OPTS) $(LDFLAGS) $(INCLUDES) -o $(TESTOUT) $(TESTSRC) $(LIBS)

tool:	../tool/ropy.c
	gcc $(OPTS) $(LDFLAGS) $(INCLUDES) -o $(TOOLOUT) $(TOOLSRC) $(LIBS)

clean:
	@/bin/rm -f *~ *.o *.a $(TESTOUT) $(TOOLOUT)
	@/bin/rm -f ../test/*~
	@/bin/rm -f ../tool/*~
	@/bin/rm -Rf ../test/testexec.dSYM/
	@/bin/rm -Rf ../tool/ropy.dSYM/
	@/bin/rm -f ../include/*~
	@/bin/rm -f ../lib/*.a
	@/bin/rm -f ../*~