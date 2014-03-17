#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char * argv[]){

	// argument check
	if(argc == 0){
		// no ip argument given
		exit(0);	
	}

	// data to be written
	char *data = argv[2];
	int datalen = strlen(data);

	struct addrinfo hints;
	struct addrinfo *ai0;
	int i;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC; 		// can use either IPv4 of IPv6
	hints.ai_socktype = SOCK_STREAM;		// want a TCP socket
	if((i = getaddrinfo(argv[1], "5000", &hints, &ai0)) != 0){
		printf("Error: unable to lookup IP address: %s", gai_strerror(i));
		return -1;
	}
	
	// ai0 is a pointer to the head of a linked list of struct addrinfo
	// values containing the possible addresses of the server; interate
	// through the list, trying to connect to each turn, stopping when
	// a connection succeeds:
	struct addrinfo *ai;
	int fd;

	for(ai = ai0; ai != NULL; ai = ai->ai_next){
		// create socket
		fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if(fd == -1){
			// Unable to create socket, try next address in list
			perror("Unable to create socket");
			continue;		
		}	
		// connect to address
		if(connect(fd, ai->ai_addr, ai->ai_addrlen) == -1){
			// couldn't connect to the address, try next in list
			perror("Unable to connect, socket closed");
			close(fd);
			continue;
		}
		break;		// successfully connnected
		printf("Connection successful");
	}
	if(ai == NULL){
		// couldn't connect to any of the addresses, handle failure
		perror("Couldn't connect to any of the addresses");	
	} else {
		// at this point, fd is a file descriptor of a socket connected
		// to the server, use the connection - write data
		if(write(fd, data, datalen) == -1){
			// Error has occurred
			perror("Error when writing data");		
		}

		printf("Message Sent\n");

		// read server data
		# define BUFLEN 1500
		ssize_t rcount;
		char buf[BUFLEN];
		rcount = read(fd, buf, BUFLEN);
		if(rcount == -1){
			// an error occurred
			perror("Error when reading data");
			return -1;	
		}
		buf[rcount] = 0;
		printf("%s", buf);	// print data
		printf("\n");
	}
	return 0;
}
