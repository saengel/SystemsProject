CC = gcc

# Commenting out original CFLAGS in attempt
# to add rio capabilities
CFLAGS = -O2 -Wall -I ..

# Adding based on echoserver to enable rio
# CSAPP_INC = ../include
# CSAPP_SRC = .. # because just inscluding the csapp in my paren directory                       
# CFLAGS = -Wall -O2 -I $(CSAPP_INC) -I .

# Adding to add rio
LIB = -lpthread

all: megillah subtract

megillah: megillah.c csapp.o
	$(CC) $(CFLAGS) -o megillah megillah.c csapp.o $(LIB)

# Wants me to add a rule to make csapp.c but not sure how
# Orginally had this below...
csapp.o: csapp.c 
	$(CC) $(CFLAGS) -c csapp.c 

subtract: subtract.c
	$(CC) $(CFLAGS) -o subtract subtract.c
clean:
	rm -f megillah subtract *~
