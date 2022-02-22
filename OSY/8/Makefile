CFLAGS+= -nostdlib -nostdinc -static -m32 -Wall -g -O2
HW=hexconv
ZIP=zip

$(HW): hexconv.c 
	$(CC) $(CFLAGS) hexconv.c -o $(HW)

zip:
	$(ZIP) $(HW)-brute.zip hexconv.c

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip

.PHONY: clean zip
