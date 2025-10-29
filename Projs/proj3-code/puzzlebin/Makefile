SHELL = /bin/bash
ARCH = $(shell uname -m)
PORT = 2021

.PHONY: test-setup test gdb zip

test-setup: puzzlebin
	@chmod u+x puzzlebin

ifeq ($(ARCH), x86_64)
test: test-setup
	./puzzlebin input.txt

gdb: test-setup
	gdb -tui puzzlebin
else
test: test-setup
	qemu-x86_64 puzzlebin input.txt

gdb: test-setup
	-@killall -r qemu* -q
	qemu-x86_64 -g $(PORT) puzzlebin input.txt &
	gdb-multiarch puzzlebin -tui -ex 'target remote localhost:$(PORT)'
endif

zip:
	@echo "ERROR: Run 'make zip' in the top-level project directory."
