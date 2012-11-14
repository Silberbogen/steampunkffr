CFLAGS  = -Wall -g -O3 -std=gnu11 
LDFLAGS = -lncursesw

OBJ = steampunkffr.o skbtools.o

steampunkffr: $(OBJ)
	        $(CC) $(CFLAGS) -o steampunkffr $(OBJ) $(LDFLAGS)

%.o: %.c
	        cc $(CFLAGS) -c $<

clean:
	rm -f steampunkffr

