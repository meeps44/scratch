# Typing "make" will execute the first rule in the makefile

IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)
# LDFLAGS=-L ../lib 

ODIR=obj
LDIR =../lib
LIBS=libext.a -lssl -lcrypto

# Function patsubst:
# Finds whitespace-separated words in TEXT that match PATTERN and replaces them
# with REPLACEMENT. Here PATTERN may contain a % which acts as a wildcard, 
# matching any number of any characters within a word.
# $(patsubst PATTERN,REPLACEMENT,TEXT)
_DEPS = ext.h main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o ext.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Make archive
extlib(..lib/ext.o): ext.o
        ar cr libext.a ext.o

# The -c flag says to generate the object file, the -o $@ says to put the 
# output of the compilation in the file named on the left side of the :, 
# the $< is the first item in the dependencies list, and the CFLAGS macro 
# is defined as above.
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# This rule will only execute if the object files exist in directory $(ODIR)
make_scratch: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# The .PHONY rule keeps make from doing something with a file named clean.
.PHONY: clean

# Clean up .o-files
# Command: make clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
