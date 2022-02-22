CFLAGS= -g -Wall
HW=forkpipe
CC=gcc

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(HW) nsd

$(HW): forkpipe.o
	gcc -o $(HW) forkpipe.c $(CFLAGS) $(EXTRA_CFLAGS)

nsd: nsd_main.o nsd.o nd.o
	gcc -o nsd nsd_main.o nsd.o nd.o $(CFLAGS) $(EXTRA_CFLAGS)

nsd_main.o: nsd_main.c
	gcc -c nsd_main.c -o nsd_main.o $(CFLAGS) $(EXTRA_CFLAGS)

nd.o: nd.c nd.h
	gcc -c nd.c -o nd.o $(CFLAGS) $(EXTRA_CFLAGS)

nsd.o: nsd.c nsd.h
	gcc -c nsd.c -o nsd.o $(CFLAGS) $(EXTRA_CFLAGS)

zip:
	tar czf osy04.tgz Makefile *.[ch]
