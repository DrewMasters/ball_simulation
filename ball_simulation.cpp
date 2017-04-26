#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
using namespace std;

void *task(void *);
static int clifd;
static char buffer[256];

int main(int argc, char **argv){
	int i,port, sock, sockfd;//, clifd;
	socklen_t clilen;
	//char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	struct hostent *server;
	string type(argv[1]);
	pthread_t *threads;

	threads = (pthread_t *)malloc(sizeof(pthread_t)*3);
	
	for (i=0; type[i]; i++) type[i] = tolower(type[i]);

	if ((argc != 3 && argc != 4) || (type != "server" && type != "client")){
		fprintf(stderr, "correct calls to program\n");
		fprintf(stderr, "	%s server port_number\n",argv[0]);
		fprintf(stderr, "	%s client port_number host_name\n",argv[0]);
		exit(1);
	}

	port = atoi(argv[2]);
	if (port <5000){
		fprintf(stderr, "port must be > 5000\n");
		exit(1);
	}

	//server code
	if (type == "server"){
		//creates an endpoint for communication
		//and returns a file descriptor to end point
		//	AF_INET= IPv4 Internet protocols
		//	SOCK_STREAM = sequenced, reliable, 2-way, connection-based byte streams
		//	0 = operating system will choose appropriate protocol
		//		TCP because it is a streaming socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0){
			fprintf(stderr, "error opening socket\n");
		}
		//initialize serv_addr buffer to zero
		bzero((char *) &serv_addr, sizeof(serv_addr));
		//setting values in sockaddr_in struct
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		//htons converts port number to network byte order
		serv_addr.sin_port = htons(port);

		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
			fprintf(stderr, "Error binding to port\n");
			exit(1);
		}
		
		//listens on socket for connecetions
		listen(sockfd, 5);
		clilen = sizeof(cli_addr);
		//blocks until client connects to server
		//returns new file descriptor after connection
		for (i=0; i<3; i++){
			clifd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if (clifd < 0) {
				fprintf(stderr,"Error on accept\n");
				exit(1);
			}
			pthread_create(&threads[i],NULL,task,NULL);
		}
		close(sockfd);
		for (i=0; i<3; i++) pthread_join(threads[i],NULL);
	}
	else if (type == "client"){
		sockfd = socket(AF_INET, SOCK_STREAM,0);
		if (sockfd < 0){
			fprintf(stderr, "error opening socket\n");
		}
		server = gethostbyname(argv[3]);
		if (server == NULL){
			fprintf(stderr, "error no such host\n");
			exit(0);
		}
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(port);
		if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 ) fprintf(stderr, "error connecting to host\n");
		if (read(sockfd, buffer, 256)<0)fprintf(stderr, "Error reading from socket\n");
		printf("%s",buffer);
		sprintf(buffer,"client connected\n");
		if (write(sockfd, buffer, strlen(buffer))<0) fprintf(stderr,"error writing to socket\n");
		sprintf(buffer,"");
		if (read(sockfd, buffer, 256)<0)fprintf(stderr, "Error reading from socket\n");
		buffer[255]='\0';
		printf("%s",buffer);
		close(sockfd);
	}

	exit(0);
}

void *task(void *){
	cout << pthread_self() << endl;
	sprintf(buffer, "Server: connected\n");
	write(clifd,buffer,strlen(buffer));
	bzero(buffer, 256);
	if (read(clifd,buffer,256) < 0) {
		fprintf(stderr, "Error reading from socket\n");
		exit(1);
	}
	cout << "in thread " <<pthread_self() << endl;
	printf("%s",buffer);
	if (write(clifd,"Got your message\n",17)<0){
		fprintf(stderr, "error writing to socket\n");
		exit(1);
	}
	close(clifd);
}
