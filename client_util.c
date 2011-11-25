#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dfs.h"

int sock;
server s;

void initClient(int argc,char *argv[])
{
	sock = createSocket();
	strcpy(s.ip_addr,argv[1]);
	s.listen_soc = LISTEN_PORT;
	createConnection(s,sock);
	
	#ifdef DEBUG
		printf("Connected to server\n");
	#endif		
}

