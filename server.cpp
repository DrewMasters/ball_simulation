#include <iostream>
#include <queue>
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

queue<string> m;

void error(string s){
	fprintf(stderr, "%s",s.c_str());
	exit(1);
}

int start_server(int port){
	/*
	 * Function gets and sets up a server for the 
	 * server's use
	 */
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
	/*
	 * Function gets, sets up, and connects to server
	 * for client's use
	 */
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
	/*
	 * Handshake function to ensure proper connection
	 */
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

void s_read_write(int c){
	/*
	 *
	 */
	int epoch;
	char buffer[256];
	string rec;

	epoch = 1;

	if (write(c,"Start",5)<0) error("error writing to socket\n");
	read(c,buffer,256);
	rec = string(buffer);
	if (rec == "awk"){
		while(rec != "exit"){
			if (!m.empty()){
				rec = m.front();
				m.pop();
				cout << "RECEIVED: " << rec << endl;
				cout << "UPDATING BASED ON " << rec << endl;
				cout << "WRITING TO CLIENT TO UPDATE" << endl;

			}
		}
	}
	else{
		error("error in starting interaction process\n");
	}

	close(c);
}

void c_read_write(int c){
	/*
	 *
	*/
	int epoch;
	
	epoch = 1;

	close(c);
}

void iq(){
	string input;
	
	getline(std::cin,input);
	m.push(input);
	input="";
}
