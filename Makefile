CFLAGS=-W -Wall -Werror

all: hello_server hello_client

hello_server: hello_server.c
	gcc ${CFLAGS} -o hello_server hello_server.c

hello_client: hello_client.c
	gcc ${CFLAGS} -o hello_client hello_client.c

clean:
	rm -f hello_server hello_client
