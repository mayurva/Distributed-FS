#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dfs.h"
int socket;
server s;

void initClient(int argc,char *argv[])
{
	socket = createSocket();
	strcpy(s.ip_addr,argv[1]);
	s.listen_soc = LISTEN_PORT;
	createConnection(server s,socket);
	
	#ifdef DEBUG
		printf("Connected to server\n");
	#endif		
}
