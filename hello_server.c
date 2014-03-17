#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

# define BUFLEN 1500 
# define PORT 5000


// server side socket creation and listen

int main(void) {
	printf("Server started...\n");

	// create socket address varaible
	struct sockaddr_in addr;

	// create socket
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		// an error occurred
		perror("Error when creating socket");
		//return -1;
	}

	// make socket into TCP server and bind socket to port
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	if(bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1){
		// an error occurred
		perror("Error when binding socket");
		return -1;
	}

	printf("Socket created and binded...\n");
	

	// start listening
	int backlog = 10;
	if(listen(fd, backlog) == -1){
		// an error occurred
		perror("Error when start listening");
		return -1;			
	}

	printf("Listening...\n");	

	// START LOOP
	while(1){ 

		// accept new connections
		int connfd;
		struct sockaddr_in cliaddr;
		socklen_t cliaddrlen = sizeof(cliaddr);
		connfd = accept(fd, (struct sockaddr *) &cliaddr, &cliaddrlen);
		if(connfd == -1){
			// an error occurred
			perror("Error when accepting socket");
			return -1;	
		}

		// read data
		ssize_t rcount;
		char buf[BUFLEN];
		rcount = read(connfd, buf, BUFLEN);
		if(rcount == -1){
			// an error occurred
			perror("Error when reading data");
			return -1;	
		}
		buf[rcount] = 0;
		printf("%s", buf);	// print data
		printf("\n");
		
		// write to client
		int datalen = strlen(buf); 
		if(write(connfd, buf, datalen) == -1){
			// Error has occurred
			perror("Error when writing data");		
		}

		// close socket
		close(connfd);
	}  // END LOOP
	return 0;
}

