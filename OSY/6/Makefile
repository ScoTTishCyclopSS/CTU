CFLAGS+= -Wall -g -O2 -std=gnu99 -pthread
HW=factory
ZIP=zip

EXTRA_CFLAGS+= -fsanitize=address -fno-omit-frame-pointer

$(HW): factory.c 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) factory.c -o $(HW)

zip:
	$(ZIP) $(HW)-brute.zip factory.c

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip

.PHONY: clean zip
