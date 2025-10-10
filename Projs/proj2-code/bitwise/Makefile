AN = proj2
CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')

.PHONY: all clean test clean-tests zip

all: ishow fshow btest ndlc

puzzle_spec.o: puzzle_spec.c puzzle_spec.h bits.h oracle.h
	$(CC) -c $<

btest.o: btest.c
	$(CC) -c $<

oracle.o: oracle.c oracle.h
	$(CC) -c $<

bits.o: bits.c bits.h
	$(CC) -c $<

btest: btest.o puzzle_spec.o oracle.o bits.o
	$(CC) -o $@ $^ -lm

ndlc:
	$(MAKE) -C ndlc_src
	mv ndlc_src/target/release/ndlc .

ishow: ishow.c
	$(CC) -o $@ $^

fshow: fshow.c
	$(CC) -o $@ $^

clean:
	$(MAKE) -C ndlc_src clean
	rm -f btest ndlc ishow fshow *.o

test: btest ndlc
	@chmod u+x testius run_tests.sh
	@./run_tests.sh $(testnum)

clean-tests:
	rm -rf test_results

zip:
	@echo "Error: Do not run make zip in the bitwise subdirectory. Run it in the project's top-level directory."
