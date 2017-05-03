#include <iostream>
#include <algorithm>
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
#include <thread>
#include "server.h"
#include <mutex>
using namespace std;

queue <string> input;

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
	if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("error binding to port\n");
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
	//close(c);
}

void server_s(int c){
	/*
	 * reads input from clients and puts it on the update queue
	 */
	cout << "in server thread\n";

	char buffer[256];
	string tmp="";

	read_write_socket(c);
	while (tmp!="exit"){
		if (read(c,buffer,256)<0) error("Error reading from socket\n");
		tmp = string(buffer);
		input.push(tmp);
		transform(tmp.begin(),tmp.end(),tmp.begin(),::tolower);
	}
}

void local_server(){
	/*
	 * Gets input from stdin from user of server program
	 * and puts it on the update queue
	 */
	cout << "in local server\n";
	char buffer[256];
	string tmp="";
	cout << "flag: " << flag << endl;
	while (tmp!="exit"){
		tmp="";
		for (int i=0; i < 256; i++) buffer[i]=0;
		cout << "about to read in input\n";
		if (read(fileno(stdin),buffer,256)<0) error("Error reading from stdin\n");
		printf("buffer: %s\n",buffer);
		cout << "input read from stdin\n";
		tmp = string(buffer);
		cout << "tmp: " << tmp << " tmp length: " << tmp.length() << endl;
		tmp.erase(tmp.length()-1);
		cout << "tmp: " << tmp << " tmp length: " << tmp.length() << endl;
		input.push(tmp);
		cout << "pushged to queue\n";
		transform(tmp.begin(),tmp.end(),tmp.begin(),::tolower);
	}
	cout << "exiting local_server\n";
	flag=1;
	cout << "flag: " << flag << endl;
}

void server_update(int c){
	/*
	 * Performs action on update queue and than sends
	 * it to cliens
	 */
	string tmp;

	while (flag==0){
		if (!input.empty()){
			tmp = input.front();
			input.pop();
			if (write(c,tmp.c_str(),256)<0) error("Error writing from socket\n");
		}
	}
}

void client_s(int c){
	/*
	 * receives input from client user and sends to server
	 */
	char buffer[256];
	string tmp="";

	while(tmp!="exit"){
		tmp="";
		for (int i=0; i<256; i++) buffer[i]=0;
		if(read(fileno(stdin),buffer,256)<0) error("Error reading from stdin\n");
		tmp = string(buffer);
		tmp.erase(tmp.length()-1);
		cout << tmp << endl;
		if(write(c,tmp.c_str(),256)<0) error("Writing to socket\n");
		transform(tmp.begin(),tmp.end(),tmp.begin(),::tolower);
	}
	flag=1;
}

void client_update(int c){
	/*
	 * receives input from server through socket
	 * takes this input and performs necessay update action
	 */
	string tmp;
	char buffer[256];

	while(flag==0){
		if(read(c,buffer,256)<0) error("Error reading from socket\n");
		tmp = string(buffer);
		cout << tmp << endl;
	}
}
