#!/usr/bin/make
.SUFFIXES:
TAR = server
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) -c  $<

server: $(OBJ)
	$(CC) $^ -o $@

all: $(TAR)

clean:
	$(RM) $(RMFILES) $(OBJ) $(TAR)
