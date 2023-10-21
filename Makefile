# Copyright Tudor Ioana-Octavia 311CAa 2021-2022

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=main

build: $(TARGETS)

main: main.c hashtable.c list.c utils.c
	$(CC) $(CFLAGS) main.c hashtable.c list.c utils.c -o main

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
