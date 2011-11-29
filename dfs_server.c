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
			struct stat stbuf;
			int res;
			printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);
			printf("Path is %s\n",path);
			memset(tcp_buf,0,MAXLEN);
			res=lstat(path,&stbuf);
			if(res == -1)
			{
				sprintf(tcp_buf,"FAIL\n%d\n",errno);	
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);	
			}
			else
			{
				printf("User id is %d\n",stbuf.st_uid);
				printf("Sent stbuf\n");
				sprintf(tcp_buf,"SUCCESS\n");	
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
				recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
				send(clientList[n].conn_socket,(char*)&stbuf,sizeof(struct stat),0);
			}
		}
		else if(strcmp(a,"MKNOD") == 0)
                {
			mode_t mode;
			dev_t rdev;
			int res;
                        
			printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);
			printf("Path is %s\n",path);
			
			memset(tcp_buf,0,MAXLEN);
			strcpy(tcp_buf,"ACK\n");
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

			recv(clientList[n].conn_socket,(char *)&mode,sizeof(mode_t),0);
			printf("Received mode\n");
			memset(tcp_buf,0,MAXLEN);
			strcpy(tcp_buf,"ACK\n");
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

			recv(clientList[n].conn_socket,(char *)&rdev,sizeof(dev_t),0);
			printf("Received dev\n");

			if (S_ISREG(mode)) {
				res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
				if (res >= 0)
					res = close(res);
			} else if (S_ISFIFO(mode))
				res = mkfifo(path, mode);
	 		else
				res = mknod(path, mode, rdev);

			memset(tcp_buf,0,MAXLEN);
			sprintf(tcp_buf,"%d",res);
			printf("Sent %s\n",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
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
 		  int res,flags;
		  printf("Received message is %s\n",a);
		  a = strtok(NULL,"\n");
		  path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
		  strcpy(path,rootpath);
		  strcat(path,a);
		  printf("Path is %s\n",path);
		  memset(tcp_buf,0,MAXLEN);
		  send(clientList[n].conn_socket,"ok",strlen("ok"),0);
		  res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		  if(res<0){
		    printf("\nError receiving flags");
		    exit(1);
		  }
		  tcp_buf[res]='\0';
		  flags=atoi(tcp_buf);
		  res = open(path,flags);
		  if (res == -1)
		    send(clientList[n].conn_socket,"success",strlen("success"),0);
		  else
		    send(clientList[n].conn_socket,"failed",strlen("failed"),0);
		  close(res);
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
                        struct stat stbuf;
			int res;
			pr	intf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			//strcat(path,"/");
			strcat(path,a);
			printf("Path is %s\n",path);
			memset(tcp_buf,0,MAXLEN);
			printf("Sent %s",tcp_buf);

			DIR *dp;
			struct dirent *de;
			printf("here1");fflush(stdout);
			dp = opendir(path);
			if (dp == NULL)
			  {
			    printf("Error during open dir");
			    // return -errno;
			  }
			send(clientList[n].conn_socket,"Start",strlen("Start"),0);
			recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
			while ((de = readdir(dp)) != NULL) {
			  struct stat st;
			  memset(&st, 0, sizeof(st));
			  st.st_ino = de->d_ino;
			  st.st_mode = de->d_type << 12;
			  send(clientList[n].conn_socket,(char*)&stbuf,sizeof(struct stat),0);
			  recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
			  send(clientList[n].conn_socket,de->d_name,strlen(de->d_name),0);
			//printf("here3");fflush(stdout);
	            }
        	 send(clientList[n].conn_socket,(char*)&stbuf,sizeof(struct stat),0);
		 recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		 send(clientList[n].conn_socket,"end",strlen("end"),0);
		 closedir(dp);
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

