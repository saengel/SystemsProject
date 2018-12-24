CC = gcc

# Commented out original CFLAGS
# CFLAGS = -O2 -Wall -I .

# Adding based on echoserver to enable rio
CSAPP_INC = ../include
CSAPP_SRC = ../src
CFLAGS = -Wall -O2 -I $(CSAPP_INC) -I .

# This flag includes the Pthreads library on a Linux box.
# Others systems will probably require something different.
LIB = -lpthread

# Trying to include csapp based on the echo makefile
# to allow me to use rio.

# Original
# all: tiny cgi

PROGS = cgi-bin/megillah\
	tiny\

all: $(CSAPP_SRC)/csapp.o $(PROGS)

tiny: tiny.c $(CSAPP_SRC)/csapp.o
	$(CC) $(CFLAGS) -o tiny tiny.c $(CSAPP_SRC)/csapp.o $(LIB)

$(CSAPP_SRC)/csapp.o: csapp.c 
	$(CC) $(CFLAGS) -c $(CSAPP_SRC)/csapp.c

cgi-bin/megillah: cgi-bin/megillah.c
	(cd cgi-bin; make)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)

