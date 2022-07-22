IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)
# LDFLAGS=-L ../lib 

ODIR=obj
LDIR =../lib
LIBS=libext.a -lssl -lcrypto

main: main.c ext.o
	${CC} -c main.c
	${CC} -o main main.o ext.o

ext.o: ext.c ext.h
	${CC} -c ext.c

libext.a: ext.o
	ar r libext.a ext.o
	ranlib libext.a