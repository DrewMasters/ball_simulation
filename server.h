int start_server(int port);
int start_client(int port, char *host);
void read_write_socket(int c);
void server_s(int c);
void client_s(int c);
void local_server();
extern int flag;
