# CSCI 2021 lab Makefile
AN = lab02

CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')

.PHONY: all clean clean-tests help zip test test-quiz test-code test-setup

all: list_main

# cleaning target to remove compiled programs/objects
clean:
	rm -f list_main *.o

clean-tests:
	rm -rf test_results

help:
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make test                     # run all tests'
	@echo '  > make test-quiz                # run tests that verify quiz answers'
	@echo '  > make test-code                # run tests that verify code is correct'

# 'make zip' to create zip file for submission
zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/testius" "$(CWD)/test_cases/*"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

%.o: %.c list.h			      # shortcut to compile all .o's from .c's
	$(CC) -c $<

list_main: list_main.o list_funcs.o list.h   # create the list_main program
	$(CC) -o $@ $^

test-setup:
	@chmod u+x testius
	@chmod u+x test_cases/resources/socrates

test-quiz: test-setup QUESTIONS.txt
	./testius test_cases/tests.json -v -n 1

test-code: test-setup list_main
	./testius test_cases/tests.json -v -n 2-5

ifdef testnum
test: test-setup list_main
	./testius test_cases/tests.json -v -n "$(testnum)"
else
test: test-setup list_main
	./testius test_cases/tests.json
endif
