# CSCI 2021 lab Makefile
AN = lab01
CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')

.PHONY: clean test test-quiz test-code test-setup

hello_c: hello_c.c
	$(CC) -o $@ $^

# cleaning target to remove compiled programs/objects
clean:
	rm -f hello_c

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
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/test_cases/*" "$(CWD)/testius"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

# testing targets below
test: test-setup hello_c
	./testius test_cases/tests.json -v

test-quiz: test-setup QUESTIONS.txt
	./testius test_cases/tests.json -n 1 -v

test-code: test-setup hello_c
	./testius test_cases/tests.json -n 2 -v

test-setup:
	@chmod u+x ./testius test_cases/resources/socrates
