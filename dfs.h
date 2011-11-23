#ifndef DFS
#define DFS

#define MAX_CLIENTS 10
#define LISTEN_PORT 5000

typedef struct client_info{
	int id;
	int conn_socket;
	pthread_t thread;
};

typedef struct server{
	char ip_addr[128];
	char iface_name[64];
	int listen_soc;
};

extern rootpath;
extern server s;
extern client_info *clientList;
extern client_info me;
#endif
