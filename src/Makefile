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

# Typing "make" will execute the first rule in the makefile
make_scratch: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# The .PHONY rule keeps make from doing something with a file named clean.
.PHONY: clean

# Clean up .o-files
# Command: make clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
