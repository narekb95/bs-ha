#!/usr/bin/make
.SUFFIXES:
TAR = client
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

CFLAGS = -O2

%.o: %.c
	$(CC) $(CFLAGS) -c $<

client: $(OBJ)
	$(CC) $^ -o $@

all: $(TAR)

clean:
	$(RM) $(RMFILES) $(OBJ) $(TAR)

debug: CFLAGS += -DDEBUG -g
debug: all
