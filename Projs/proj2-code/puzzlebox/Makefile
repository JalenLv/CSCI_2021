CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)

puzzlebox: puzzlebox.c
	$(CC) -o $@ $^

test: puzzlebox input.txt
	./puzzlebox input.txt

clean:
	rm -f puzzlebox

zip:
	@echo "Error: Do not run make zip in the puzzlebox subdirectory. Run it in the project's top-level directory."
