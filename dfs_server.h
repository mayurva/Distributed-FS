#ifndef DFS_SERVER
#define DFS_SERVER
typedef struct client_info{
	int id;
	int conn_socket;
};

typedef struct server{
	char ip_addr[128];
	char iface_name[64];
	int listen_port;
};

extern server s;

#endif
