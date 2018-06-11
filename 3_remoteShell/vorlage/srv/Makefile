#!/usr/bin/make
.SUFFIXES:
TAR = server
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

CFLAGS = -O2

%.o: %.c
	$(CC) $(CFLAGS) -c $<

server: $(OBJ)
	$(CC) $^ -o $@

all: $(TAR)

clean:
	$(RM) $(RMFILES) $(OBJ) $(TAR)

debug: CFLAGS += -DDEBUG -g
debug: all
