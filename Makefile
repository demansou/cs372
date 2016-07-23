CFLAGS = -Wall -fpic -coverage -lm

chatclient: chatclient.c
	gcc -o chatclient chatclient.c $(CFLAGS)

all: chatclient

clean:
	rm -f chatclient
