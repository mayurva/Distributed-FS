#ifndef DFS
#define DFS

#include<pthread.h>


#define DEBUG
#define MAX_CLIENTS 10
#define LISTEN_PORT 5000
#define MAXLEN 1000

typedef struct client_info_{
	int id;
	int conn_socket;
	pthread_t thread;
} client_info;

typedef struct server_{
	char ip_addr[128];
	char iface_name[64];
	int listen_soc;
} server;

extern char *rootpath;
extern server s;
extern client_info *clientList;
extern int sock;

server populatePublicIp(server);

#endif
