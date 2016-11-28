CC = g++
DEBUG = -g
CFLAGS = -Wall $(DEBUG)

default :
	    $(CC) $(CFLAGS) -o server server.c; \
		$(CC) $(CFLAGS) -o client client.c

clean:
	    \rm server client

tar:
	    tar czfv socket_solution.tar.gz client.c server.c Makefile
