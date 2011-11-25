#include<stdio.h>
#include<string.h>
#include"dfs.h"

char tcp_buf[MAXLEN];

void* processClient(void* clientptr)
{
	int n = *((int*)clientptr);
//	client_info client = clientList[n];

	#ifdef DEBUG
		printf("Connected to client %d\n",n);
	#endif	

	//send initial data structures if any

	//wait for receiving the requests	
	while(1)
	{
		char *a;
		recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		a = strtok(tcp_buf,"\n");
		if(strcmp(a,"GETATTR") == 0)
		{
			printf("Received message is %s\n",a);
			memset(tcp_buf,0,MAXLEN);
			strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
		}
		else if(strcmp(a,"MKNOD") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"MKDIR") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"OPEN") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"READ") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"WRITE") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"GETDIR") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"ACCESS") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"CHMOD") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"CHOWN") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"RMDIR") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"RENAME") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"SYMLINK") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"LINK") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
               else if(strcmp(a,"UNLINK") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                }
	}
}

int main()
{
        int i=0,j;
        initServer();
        while(i<MAX_CLIENTS)
        {
                clientList[i].conn_socket = acceptConnection(s.listen_soc);
                clientList[i].id = i;

                #ifdef DEBUG
                        printf("Connected to client %d\n",i);
                #endif
		j=i;
                pthread_create(&clientList[i].thread,NULL,processClient,(void*)&j);
                i++;

        }

        for(j=0;j<i;j++)
                pthread_join(clientList[j].thread,NULL);

        return 0;
}

