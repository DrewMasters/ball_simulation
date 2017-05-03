int start_server(int port);
int start_client(int port, char *host);
void read_write_socket(int c);
void server_s(int c);
void client_s(int c);
void client_update(int c);
void local_server();
void server_update(int c);
extern int flag;
