#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dfs.h"

int sock;
server s;
char *rootpath = "/tmp/dfs";

void initClient(int argc,char *argv[])
{
	sock = createSocket();
	strcpy(s.ip_addr,argv[1]);
	s.listen_soc = LISTEN_PORT;
	createConnection(s,sock);
//	cachepath = malloc(strlen("/tmp");
	#ifdef DEBUG
		printf("Connected to server\n");
	#endif		
}
