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
	 * function to test for messages from socket 
	 * if there is a message than read that message and perform tasks on it
	 * next check to see if there has been any input messages
	 * if there are input messages than send them
	 */
	int ret;
	fd_set rs;
	string reply;

	FD_ZERO(&rs);
	FD_SET(c, &rs);

	while(reply != "exit"){
		//cout << "in while\n";
		ret = select(c+1,&rs,NULL,NULL,NULL);
		if (ret > 0){
			//cout << "in if (ret >0)\n";
			if (FD_ISSET(c,&rs)){
				//cout << "about to read from socket\n";
				ret = read(c,&reply[0],256);
				//cout << ret << endl;
				//if (ret == 0) reply = "exit";
				//cout << reply << endl;
			}
		}
		if (!m.empty()){
			reply = m.front();
			m.pop();
			cout << "writing " << reply << endl;
			if (write(c,reply.c_str(),strlen(reply.c_str()))<0) error("error writing to socket\n");
			cout << "wrote " << reply << endl;
		}
		else{
//			cout << "	nothing to write\n";
		}
	}
	close(c);
}

void iq(){
	string input;
	
	getline(std::cin,input);
	m.push(input);
	input="";
}
