#include <iostream>
#include <stdio.h>
#include <queue>
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
#include <vector>
#include <mutex>
using namespace std;

int flag = 0;
queue <string> input;
int client_count=0;
int current_count = 0;
mutex count_lock;

int main(int argc, char **argv){
	int i,port, sock, sockfd, clifd;
	vector <thread> clients;
	vector <int> cli;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in  cli_addr;
	struct hostent *server;
	string type;
	thread *c_s,*l_s;;

	if (argc == 3 || argc ==4) type = string(argv[1]);
	else type = "error";
	
	if ((argc != 3 && argc != 4) || (type != "server" && type != "client")){
		fprintf(stderr, "correct calls to program\n");
		fprintf(stderr, "	%s server port_number\n",argv[0]);
		fprintf(stderr, "	%s client port_number host_name\n",argv[0]);
		exit(1);
	}

	for (i=0; type[i]; i++) type[i] = tolower(type[i]);

	port = atoi(argv[2]);
	if (port <5000){
		fprintf(stderr, "port must be > 5000\n");
		exit(1);
	}

	//server code
	if (type == "server"){
		vector <thread> t;

		sockfd = start_server(port);
		clilen = sizeof(cli_addr);

		l_s = new thread(local_server);

		//blocks until client connects to server
		//returns new file descriptor after connection
		for ( ; ; ){
			clifd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			cout << "new connection\n";
			if(flag==1){
				l_s->join();
				cout << "server exited\n";
				break;
			}
			if (clifd < 0) {
				fprintf(stderr,"Error on accept\n");
				exit(1);
			}
			//c_s = new thread(server_s, clifd);
			cout << "about to create thread\n";
			cout << "created thread\n";
			clients.push_back(thread(server_s,clifd));
			cout << "stored thread\n";
			cout << "adding clifd to vector\n";
			cli.push_back(clifd);
			client_count++;
			cout << "added clifd to vector " << clifd << endl;
			t.push_back(thread(server_update,clifd));
			/*for (i = 0; i < clients.size(); i++){
				if(clients[i].joinable()){
					cout << "i: " << i << " is joinable\n";
					clients[i].join();
					cout << i << " joined\n";
					clients.erase(clients.begin()+i);
					cout << i << " erased\n";
				}
			}*/
			cout << "at end\n";
			if(flag==1){
				l_s->join();
				break;
			}
		}
		close(sockfd);
		cout << "closed socket fd\n";
		cout << "threads joined\n";
	}
	else if (type == "client"){
		sockfd = start_client(port,argv[3]);
		//cout << "started client " << sockfd << endl;
		if (read(sockfd, buffer, 256)<0) fprintf(stderr, "Error reading from socket\n");
		printf("%s",buffer);
		sprintf(buffer,"client connected\n");
		if (write(sockfd, buffer, strlen(buffer))<0) fprintf(stderr,"error writing to socket\n");
		sprintf(buffer,"");
		if (read(sockfd, buffer, 256)<0) fprintf(stderr, "Error reading from socket\n");
		buffer[255]='\0';
		printf("%s",buffer);
		//write(sockfd,"exit",4);

		c_s = new thread(client_s,sockfd);
		l_s = new thread(client_update,sockfd);

		cout << "created thread\n";
		cout << "thread joined\n";		
		
		c_s->join();
		l_s->join();

		close(sockfd);
	}

	exit(0);
}
