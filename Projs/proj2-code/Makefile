AN = proj2
SHELL = /bin/bash

.PHONY: all clean clean-tests bitwise puzzlebox test-bitwise test-puzzlebox zip

all: bitwise puzzlebox

test: test-bitwise test-puzzlebox

bitwise:
	$(MAKE) -C bitwise

puzzlebox:
	$(MAKE) -C puzzlebox

test-bitwise:
	$(MAKE) -C bitwise test

test-puzzlebox:
	$(MAKE) -C puzzlebox test

zip: clean clean-tests
	rm -f $(AN)-code.zip
	zip -r $(AN)-code.zip *
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

clean:
	$(MAKE) -C bitwise clean
	$(MAKE) -C puzzlebox clean

clean-tests:
	$(MAKE) -C bitwise clean-tests
