CFLAGS+= -Wall -g -O2 -std=c99 -pthread
HW=prod-cons
ZIP=zip

EXTRA_CFLAGS+= -fsanitize=address -fno-omit-frame-pointer

$(HW): main.c 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) main.c -o $(HW)

zip:
	$(ZIP) $(HW)-brute.zip main.c

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip

.PHONY: clean zip
