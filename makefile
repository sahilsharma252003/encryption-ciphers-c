CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGETS = scrypt vcrypt feistel 

all: $(TARGETS)
	chmod u+x $(TARGETS)

scrypt: scrypt.c
	$(CC) $(CFLAGS) -o scrypt scrypt.c

vcrypt: vcrypt.c
	$(CC) $(CFLAGS) -o vcrypt vcrypt.c

feistel: feistel.c
	$(CC) $(CFLAGS) -o feistel feistel.c


clean:
	rm -f $(TARGETS)
