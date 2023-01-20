CFLAGS+= -pedantic -Wall -std=c99 -O3
HW=hw4
ZIP=zip
CC=gcc

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(HW): main.c q.o g.o
	$(CC) $(CFLAGS) main.c g.o q.o -o $(HW)

q.o: q.c q.h
	gcc -c q.c -o q.o $(CFLAGS)

g.o: g.c g.h
	gcc -c g.c -o g.o $(CFLAGS)

zip:
	$(ZIP) $(HW)-brute.zip main.c

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip

.PHONY: clean zip
