# CSCI 2021 lab Makefile
AN = lab09

CFLAGS = -Wall -Werror -g -Og
CC = gcc $(CFLAGS)
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')

.PHONY: zip test-setup test-quiz test-code test clean clean-tests help

struct_stride: struct_stride.c
	$(CC) -o $@ $^

help:
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make test-quiz                # run tests that verify quiz answers'
	@echo '  > make test-code                # run tests that verify code is correct'
	@echo '  > make test                     # run all automated tests'
	@echo '  > make clean-tests              # remove all test results'

zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/testius" "$(CWD)/test_cases/*"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

test-setup:
	@chmod u+rx testius
	@chmod u+rx test_cases/resources/socrates

test-quiz: test-setup
	./testius test_cases/tests.json -v -n 1

test-code: test-setup struct_stride
	./testius test_cases/tests.json -v -n 2

test: test-setup struct_stride
	./testius test_cases/tests.json

clean:
	rm -f struct_stride

clean-tests:
	rm -rf test_results/
