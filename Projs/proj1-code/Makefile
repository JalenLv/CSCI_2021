CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
SHELL =/bin/bash
CWD = $(shell pwd | sed 's/.*\///g')
AN = proj1

TEMPORARY_FILES = \
	csci_2021.txt \
	csci_2021.bin \
	MATH1572.txt \
	MATH1573.bin \
	csci4131.txt \
	csci_5521.bin \
	csci_5521.txt \
	csci4131.bin

.PHONY: all test clean clean-tests zip help

all: gradebook_main

gradebook.o: gradebook.c gradebook.h
	$(CC) -c $<

gradebook_main.o: gradebook_main.c gradebook.h
	$(CC) -c $<

gradebook_main: gradebook_main.o gradebook.o
	$(CC) -o $@ $^

test-setup:
	@rm -rf $(TEMPORARY_FILES)
	@cp test_cases/resources/csci4131.txt .
	@cp test_cases/resources/csci_5521.bin .
	@chmod u-w csci4131.txt
	@chmod u-w csci_5521.bin
	@chmod u+x testius

ifdef testnum
test: gradebook_main test-setup
	./testius test_cases/tests.json -v -n $(testnum)
else
test: gradebook_main test-setup
	./testius test_cases/tests.json
endif

clean:
	rm -f *.o gradebook_main

clean-tests:
	rm -rf test_results $(TEMPORARY_FILES)

zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/testius" "$(CWD)/test_cases/*"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

help:
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make clean-tests              # remove temporary testing files'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make test                     # run all tests'
	@echo '  > make test testnum=n           # run only test number n'
