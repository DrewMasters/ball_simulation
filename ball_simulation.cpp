#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

int main(int argc, char **argv){
	int port, sock, sockfd, clifd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	string type(argv[2]);

	port = atoi(argv[1]);
	if (port <5000){
		fprintf(stderr, "port must be > 5000\n");
		exit(1);
	}
	
	for (int i=0; type[i]; i++) type[i] = tolower(type[i]);

	//server code
	if (type == "server"){
		//creates an endpoint for communication
		//and returns a file descriptor to end point
		//	AF_INET= IPv4 Internet protocols
		//	SOCK_STREAM = sequenced, reliable, 2-way, connection-based byte streams
		//	0 = operating system will choose appropriate protocol
		//		TCP because it is a streaming socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		//initialize serv_addr buffer to zero
		bzero((char *) &serv_addr, sizeof(serv_addr));
		//setting values in sockaddr_in struct
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		//htons converts port number to network byte order
		serv_addr.sin_port = htons(port);

		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) fprintf(stderr, "Error binding to port\n");

		//listens on socket for connections
		listen(sockfd, 5);
		clilen = sizeof(cli_addr);
		//blocks until client connects to server
		//returns new file descriptor after connection
		clifd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (clifd < 0) fprintf(stderr,"Error on accept\n");

		printf("Connection esablished\n");
		sprintf(buffer, "Server: connected\n");
		write(clifd,buffer,strlen(buffer));	
		printf("Receiving\n");
		
		bzero(buffer, 256);
		if (read(clifd,buffer,256) < 0) fprintf(stderr, "Error reading from socket\n");
		printf("%s",buffer);
	}
	else if (type == "client"){

	}
	else{
		fprintf(stderr, "incorrect function input\n");
		fprintf(stderr, "server or client are the only accepted types\n");
		exit(1);
	}

	exit(0);
}
