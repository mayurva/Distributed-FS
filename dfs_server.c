#include<stdio.h>
#include"dfs.h"

int main()
{
	int i=0,j;
	initServer();
	while(i<MAX_CLIENTS)
	{
		clientList[i].conn_socket = acceptConnection();
		clientList[i].id = i;
		
		#ifdef DEBUG
			printf("Connected to client %d\n",i);
		#endif
		pthread_create(&clientList[i].thread,NULL,processClient,(void*)&clientList[i]);	
		i++;
		
	}

	for(j=0;j<i;j++)
		pthread_join(clientList[j].thread,NULL);

	return 0;
}

void* processClient(void* clientptr)
{
	client_info client = *clientptr;

	#ifdef DEBUG
		printf("Connected to client %d\n",client.id);
	#endif


	//send initial data structures if any

	//wait for receiving the requests	
}
