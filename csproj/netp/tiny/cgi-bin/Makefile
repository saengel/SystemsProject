CC = gcc
CFLAGS = -O2 -Wall -I ..

all: megillah subtract

megillah: megillah.c
	$(CC) $(CFLAGS) -o megillah megillah.c

subtract: subtract.c
	$(CC) $(CFLAGS) -o subtract subtract.c
clean:
	rm -f megillah subtract *~
