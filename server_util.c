#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dfs.h"

char *rootpath;
server s;
client_info *clientList;

void initServer()
{
	rootpath = (char*)get_current_dir_name();
	s = populatePublicIp(s);
	s.listen_soc = createSocket();
	bindSocket(s.listen_soc,LISTEN_PORT,s.ip_addr);
	clientList = (client_info *)malloc(sizeof(client_info)*MAX_CLIENTS);
	listenSocket(s.listen_soc);

	#ifdef DEBUG
		printf("Server initialized\n");
		printf("Root dir is %s\n",rootpath);
	#endif
}
