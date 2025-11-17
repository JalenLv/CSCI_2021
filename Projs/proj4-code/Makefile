CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')
AN = proj4

all: el_demo test_el_malloc

el_demo: el_malloc.o el_demo.o
	$(CC) -o $@ $^

el_malloc.o: el_malloc.c el_malloc.h
	$(CC) -c $<

el_demo.o: el_demo.c el_malloc.h
	$(CC) -c $<

test_el_malloc: test_el_malloc.o el_malloc.o
	$(CC) -o $@ $^

test_el_malloc.o: test_el_malloc.c el_malloc.h
	$(CC) -c $<

clean:
	rm -f test_el_malloc el_demo *.o

help:
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make test                     # run all tests'
	@echo '  > make test testnum=5           # run problem 1 test #5 only'

zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/testius" "$(CWD)/test_cases/*"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

ifdef testnum
test: test-setup test_el_malloc
	./testius test_cases/tests.json -v -n "$(testnum)"
else
test: test-setup test_el_malloc
	./testius test_cases/tests.json
endif

test-setup:
	@chmod u+rx testius

clean-tests:
	rm -rf test_results
