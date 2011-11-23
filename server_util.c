#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *rootpath;
server s;
client_info *clientList;

void initServer()
{
	rootpath = get_current_dir_name();
	s = populatePublicIp(s);
	s.listen_soc = createSocket();
	bindSocket(s.listen_soc,LISTEN_PORT,s.ip_addr);
	clientList = (clientList *)malloc(sizeof(clientInfo)*MAX_CLIENTS);
	listenSocket(s.listen_soc);

	#ifdef DEBUG
		printf("Server initialized\n");
		printf("Root dir is %s\n",rootpath);
	#endif
}
