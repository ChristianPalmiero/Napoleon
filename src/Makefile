CC=arm-linux-gnueabi-gcc
CFLAGS=-O3 -W -Wall -Wno-comment -std=gnu99 -I../../ev3dev-c/source/ev3 
LIBS=-lm -lev3dev-c -lpthread
ODIR=../obj

DEPS= engines.h sensors.h position.h eye.h nav.h
_OBJ= main.o engines.o sensors.o position.o eye.o nav.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o
	rm -f $(ODIR)/main

