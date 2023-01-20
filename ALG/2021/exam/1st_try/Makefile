CFLAGS+=  -pedantic -Wall -std=c99 -O0
HW=exam
ZIP=zip

$(HW): main.c 
	$(CC) $(CFLAGS) main.c -o $(HW)

zip:
	$(ZIP) $(HW)-exam.zip main.c

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-exam.zip

.PHONY: clean zip
