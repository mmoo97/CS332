# Sample Makefile to compile C programs

CC = gcc
CFLAGS = -Wall -O#replace -g with -O when not debugging 

DEPS	= queue.h Makefile
OBJS	= queue.o qdriver.o blazersh.o

EXECS	= blazersh

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

blazersh:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:	
	/bin/rm -i *.o $(EXECS)
# CC=gcc
# CFLAGS=-I.

# blazersh: blazersh.o
# 	$(CC) -o blazersh blazersh.o -I "."
