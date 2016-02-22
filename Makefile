CFLAGS  = -Wall -g -O3 -std=gnu11 
LDFLAGS = -lncursesw

OBJ = steampunkffr.o skbtools.o

all: steampunkffr

steampunkffr: $(OBJ)
	        $(CC) $(CFLAGS) -o steampunkffr $(OBJ) $(LDFLAGS)

%.o: %.c
	        cc $(CFLAGS) -c $<

clean:
	rm -f steampunkffr

install: all
	@echo Installiere steampunkffr nach /usr/local/bin
	@mkdir -p /usr/local/bin
	@cp -f steampunkffr /usr/local/bin
	@chmod 755 /usr/local/bin/steampunkffr

uninstall:
	@echo Entferne steampunkffr aus /usr/local/bin
	@rm -f /usr/local/bin/steampunkffr
