CFLAGS = -Wall -g -Og
CC = gcc $(CFLAGS)

.PHONY: all clean

all: matsums_main

matsums_main: matsums_main.o matsums_funcs.o matvec_util.o
	$(CC) -o $@ $^

matsums_main.o: matsums_main.c matvec.h
	$(CC) -c $<

matsums_funcs.o: matsums_funcs.c matvec.h
	$(CC) -c $<

matvec_util.o: matvec_util.c matvec.h
	$(CC) -c $<

clean:
	rm -f *.o matsums_main
