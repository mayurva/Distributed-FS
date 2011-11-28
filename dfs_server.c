#include<stdio.h>
#include<string.h>
#include"dfs.h"

pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recv_mutex = PTHREAD_MUTEX_INITIALIZER;
char tcp_buf[MAXLEN];

void* processClient(void* clientptr)
{
	int n = *((int*)clientptr) ;
	char *path;
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
		printf("Recvd %d\n",n);
		a = strtok(tcp_buf,"\n");
		printf("%s\n",a);
		if(strcmp(a,"GETATTR") == 0)
		{
			struct stat *stbuf;
			int res;
			printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
//			strcat(path,"/");
			strcat(path,a);
			printf("Path is %s\n",path);
			memset(tcp_buf,0,MAXLEN);
			res=lstat(path,stbuf);
			printf("Sent stbuf\n");
			send(clientList[n].conn_socket,(char*)stbuf,sizeof(struct stat),0);
		}
		else if(strcmp(a,"MKNOD") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"MKDIR") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"OPEN") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"READ") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"WRITE") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"GETDIR") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"ACCESS") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"CHMOD") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"CHOWN") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"RMDIR") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"RENAME") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"SYMLINK") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex); 
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"LINK") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
                }
               else if(strcmp(a,"UNLINK") == 0)
                {
                        printf("Received message is %s\n",a);
                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			pthread_mutex_lock(&send_mutex);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			pthread_mutex_unlock(&send_mutex);
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

