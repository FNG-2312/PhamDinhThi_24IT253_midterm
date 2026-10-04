CC = gcc
CFLAGS = -Wall -Wextra -g

all: myls

myls: main.c ls.c
	$(CC) $(CFLAGS) -o myls main.c ls.c

clean:
	rm -f myls
