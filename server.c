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

void error(string s){
	fprintf(stderr, "%s",s.c_str());
	exit(1);
}

int start_server(int port){
	cout << "in start_server\n";
	struct sockaddr_in serv_addr;
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("error opening socket\n");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("error binding to port\n");
	listen(sockfd,5);	
	return sockfd;
};

int start_client(int port, char *host){
	cout << "in start_client\n";
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int sockfd;

	server = gethostbyname(host);
	if (server == NULL) error("error no such host\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("error opening socket\n");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	if(::connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("error connecting to host\n");
	return sockfd;
}

void read_write_socket(int c){
	char buffer[256];

	cout << pthread_self() << endl;
	sprintf(buffer, "Server connected\n");
	write(c,buffer,strlen(buffer));
	bzero(buffer, 256);
	if (read(c,buffer,256) < 0) {
		fprintf(stderr, "Error reading from socket\n");
		exit(1);
	}
	cout << "in thread " <<pthread_self() << endl;
	printf("%s",buffer);
	if (write(c,"Got your message\n",17)<0){
		fprintf(stderr, "error writing to socket\n");
		exit(1);
	}
	close(c);
}
