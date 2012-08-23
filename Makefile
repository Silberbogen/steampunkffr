CFLAGS  = -Wall -g -std=c99
LDFLAGS = -lncursesw

OBJ = steampunkffr.o skbtools.o

steampunkffr: $(OBJ)
	        $(CC) $(CFLAGS) -o steampunkffr $(OBJ) $(LDFLAGS)

%.o: %.c
	        cc $(CFLAGS) -c $<

clean:
	rm -f steampunkffr

