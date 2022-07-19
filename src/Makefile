IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)
# LDFLAGS=-L ../lib 

ODIR=obj
LDIR =../lib
LIBS=libext.a -lssl -lcrypto

_DEPS = ext.h main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o ext.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

scratch: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
