#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include "server.h"
using namespace std;

void *task(void *);
void read_write_socket(int c);
static int clifd;
static char buffer[256];

int main(int argc, char **argv){
	int i,port, sock, sockfd;//, clifd;
	socklen_t clilen;
	//char buffer[256];
	struct sockaddr_in  cli_addr;
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
		sockfd = start_server(port);
		cout << "started server " << sockfd << endl;
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
		sockfd = start_client(port,argv[3]);
		cout << "started client " << sockfd << endl;
		if (read(sockfd, buffer, 256)<0)fprintf(stderr, "Error reading from socket\n");
		printf("%s",buffer);
		sprintf(buffer,"client connected\n");
		if (write(sockfd, buffer, strlen(buffer))<0) fprintf(stderr,"error writing to socket\n");
		sprintf(buffer,"");
		if (read(sockfd, buffer, 256)<0)fprintf(stderr, "Error reading from socket\n");
		buffer[255]='\0';
		printf("%s",buffer);
		
		//read_write_socket(sockfd);
		close(sockfd);
	}

	exit(0);
}

void *task(void *){
	read_write_socket(clifd);
}
