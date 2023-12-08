CC = gcc
CFLAGS = -Wall

all: client server parser fonctionnalite

client: client.o
	$(CC) -o client client.o

server: server.o
	$(CC) -o server server.o

parser: parser.o
	$(CC) -o parser parser.o

fonctionnalite: fonctionnalite.o
	$(CC) -o fonctionnalite fonctionnalite.o

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

fonctionnalite.o: fonctionnalite.c
	$(CC) $(CFLAGS) -c fonctionnalite.c

clean:
	rm -f client server parser fonctionnalite *.o

