SHELL = /bin/bash
ARCH = $(shell uname -m)
PORT = 2021

CCFLAGS = -Wall -Werror -g
ifeq ($(ARCH), x86_64)
	CC = gcc $(CCFLAGS)
else
	CC = x86_64-linux-gnu-gcc $(CCFLAGS)
endif

.PHONY: all clean clean-tests test gdb zip

all: ishow fshow btest

puzzle_spec.o: puzzle_spec.c puzzle_spec.h bits.h oracle.h
	$(CC) -c $<

btest.o: btest.c
	$(CC) -c $<

oracle.o: oracle.c oracle.h
	$(CC) -c $<

bits.o: bits.s bits.h
	$(CC) -c $<

btest: btest.o puzzle_spec.o oracle.o bits.o
	$(CC) -o $@ $^ -lm

ishow: ishow.c
	$(CC) -o $@ $^

fshow: fshow.c
	$(CC) -o $@ $^

clean:
	rm -f btest ishow fshow *.o

clean-tests:
	rm -rf test_results

test: btest
	@chmod u+x testius
	@chmod u+x run_tests.sh
	./run_tests.sh $(testnum)

ifeq ($(ARCH), x86_64)
gdb: btest
	gdb btest
else
gdb: btest
	-@killall -r qemu* -q
	qemu-x86_64 -g $(PORT) btest &
	gdb-multiarch btest -ex 'target remote localhost:$(PORT)'
endif

zip:
	@echo "ERROR: Run 'make zip' in the top-level project directory."
