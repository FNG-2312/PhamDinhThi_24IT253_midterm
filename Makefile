CC = gcc
CFLAGS = -Wall -Wextra -g

all: myls

myls: main.c
	$(CC) $(CFLAGS) -o myls main.c

clean:
	rm -f myls
