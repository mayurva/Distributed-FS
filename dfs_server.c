#include<stdio.h>
#include<string.h>
#include"dfs.h"

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
		memset(tcp_buf,0,MAXLEN);
		recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
//		printf("Recvd %d\n%s\n",n,tcp_buf);
		a = strtok(tcp_buf,"\n");
		if(a)
		{
		printf("%s\n",a);
		if(strcmp(a,"GETATTR") == 0)
		{
			printf("inside getattr\n");
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
			free(path);
			printf("getattr ends\n");
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
			memset(tcp_buf,0,MAXLEN);
			printf("mknod ends\n");
			free(path);	
                }
               else if(strcmp(a,"MKDIR") == 0)
                {
			int res;
			mode_t mode;

                        printf("Received message is %s\n",a);
			
			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);
			printf("Path is %s\n",path);

                        memset(tcp_buf,0,MAXLEN);
			strcpy(tcp_buf,"ACK\n");
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

			recv(clientList[n].conn_socket,(char *)&mode,sizeof(mode_t),0);
                        printf("Received mode\n");
			res = mkdir(path, mode);

                        memset(tcp_buf,0,MAXLEN);
			if (res == -1)
				sprintf(tcp_buf,"FAIL\n%d\n",-errno);
			else
				sprintf(tcp_buf,"SUCCESS\n%d\n",0);
                
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			free(path);
			memset(tcp_buf,0,MAXLEN);
                }
		else if(strcmp(a,"OPEN") == 0)
                {
 		  int res,flags,tot=0;
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
		  if (res != -1)
		    send(clientList[n].conn_socket,"success",strlen("success"),0);
		  else
		    send(clientList[n].conn_socket,"failed",strlen("failed"),0);	
		  close(res);
		free(path);
		memset(tcp_buf,0,MAXLEN);
		printf("open ends\n");
                }
		else if(strcmp(a,"READ") == 0)
                {
			int res,fd,flags,tot=0;
			printf("Received message is %s\n",a);fflush(stdout);
			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);
			printf("Path is %s\n",path);fflush(stdout);
			memset(tcp_buf,0,MAXLEN);
			send(clientList[n].conn_socket,"ok",strlen("ok"),0);
		  
			res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
			if(res<0){
				printf("\nError receiving flags");
		    		exit(1);
		  	}
			tcp_buf[res]='\0';
		  	flags=atoi(tcp_buf);
		  	fd = open(path,flags);
		  	if (fd != -1)	//file present. now read required number of blocks
		    	{
				int breakflag = 0;
				char readbuf[MAXLEN];
		      		send(clientList[n].conn_socket,"success",strlen("success"),0);
		      		memset(tcp_buf,0,MAXLEN);
		      		res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		      		if(res<0){
					printf("\nError receiving flags");
					exit(1);
		      		}
			      	tcp_buf[res]='\0';

		      		int offset=atoi(tcp_buf);
				printf("offset is %d\n",offset);
		      		memset(tcp_buf,0,MAXLEN);
		      		strcpy(tcp_buf,"NEXT");
		      		printf("reading 1st time\n");fflush(stdout);		      
				while(strcmp(tcp_buf,"NEXT")==0)
				{
			  		//printf("here3");fflush(stdout);
			  		memset(readbuf,0,BLOCKSIZE);
			  		int rflag=pread(fd,readbuf,BLOCKSIZE,offset);
			  		printf("data read\n%s",readbuf);fflush(stdout);
			  	        if(rflag==0)
			                {
						memset(tcp_buf,0,MAXLEN);
						sprintf(tcp_buf,"FINISH\n%d\n",tot);
						printf("finished reading\nbytes read is %d\n",tot);
                  				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);	//send no of bytes read
						breakflag = 1;	
						break;
                			}
              				else if(rflag==-1)
                			{
						memset(tcp_buf,0,MAXLEN);
						sprintf(tcp_buf,"ERROR\n%d\n",-errno);
						printf("read error\n");
                  				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
						breakflag = 1;
						break;
                			}
              				else
                			{
						memset(tcp_buf,0,MAXLEN);
						sprintf(tcp_buf,"NEXT\n%s\n",readbuf);
						send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
	                  			tot=tot+rflag;
						printf("reply to client with read data\n");
                			}
                			
			  		memset(tcp_buf,0,MAXLEN);
			  		res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
			  		if(res<0){
			    			printf("\nError receiving flags");
			    			exit(1);
			  		}
			  		tcp_buf[res]='\0';
			  		printf("next message from client is %s\n",tcp_buf);fflush(stdout);
			  		offset=offset+BLOCKSIZE;
				}
				if(!breakflag)
				{
					printf("finished reading\n");
					memset(tcp_buf,0,MAXLEN);
					sprintf(tcp_buf,"%d",tot);
                  			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
					printf("sent bytes read %d\n",tot);
				}
		      		printf("here6");fflush(stdout);
		      		memset(tcp_buf,0,MAXLEN);
		      		//sleep(10);
			  	close(fd);
		    	}
		  	else
		    	{
				memset(tcp_buf,0,MAXLEN);
				sprintf(tcp_buf,"ERROR\n%d\n",-errno);
				printf("error opening file\n");
                  		send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
		    	}
		     	printf("here4");fflush(stdout);
			
		      	memset(tcp_buf,0,MAXLEN);
			printf("End of read\n");
			free(path);

                }
		else if(strcmp(a,"WRITE") == 0)
                {
		  	int res,fd,flags,tot=0;
		  	printf("Received message is %s\n",a);fflush(stdout);
		  	a = strtok(NULL,"\n");
		  	path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
		  	strcpy(path,rootpath);
		  	strcat(path,a);
		  	printf("Path is %s\n",path);fflush(stdout);
		  	memset(tcp_buf,0,MAXLEN);
		  	send(clientList[n].conn_socket,"ok",strlen("ok"),0);
		  
		  	res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		  	if(res<0){
		    		printf("\nError receiving flags");
		    		exit(1);
		  	}
		  	tcp_buf[res]='\0';
		  	flags=atoi(tcp_buf);
		  	fd = open(path,O_WRONLY);
		  	if (fd != -1)
		    	{
				char *b;
				printf("file opened\n");
				int breakflag = 0;
		      		send(clientList[n].conn_socket,"success",strlen("success"),0);
		      		memset(tcp_buf,0,MAXLEN);
		      		res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		      		if(res<0){
					printf("\nError receiving flags");
					exit(1);
		      		}
		      		tcp_buf[res]='\0';

		      		off_t offset=(off_t)atoi(tcp_buf);
				printf("got the offset\n");
		      		send(clientList[n].conn_socket,"NEXT",strlen("NEXT"),0);
		      		//memset(tcp_buf,0,MAXLEN);
		      		//res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		      		//if(res<0){
				//	printf("\nError receiving flags");
				//	exit(1);
		      		//}
		      		//tcp_buf[res]='\0';
		      		//size_t size= (size_t)atoi(tcp_buf);
		      		//send(clientList[n].conn_socket,"success",strlen("success"),0);

			      	memset(tcp_buf,0,MAXLEN);
			      	//char *recvbuf=(char *)malloc(sizeof(char)*(size+5));
		      		recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
				tot = 0;
				printf("start writing\n");

				while(strcmp(tcp_buf,"END")!=0)
				{
					a = strtok_r(tcp_buf,"\n",&b);
					//a = strtok(NULL,"\n");	
			      		//int offset=atoi(tcp_buf);
					//strcpy(recvbuf,tcp_buf);
		      			res = pwrite(fd, b, strlen(b), offset);
					printf("%d bytes written\n",res);
		      			if (res == -1)
					{
						memset(tcp_buf,0,MAXLEN);
						sprintf(tcp_buf,"ERROR\n%d\n",-errno);
		    				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
						breakflag = 1;
						break;
					}
				 	send(clientList[n].conn_socket,"NEXT",strlen("NEXT"),0);
					offset = offset + BLOCKSIZE;
					tot += res;

			      		memset(tcp_buf,0,MAXLEN);
			      		//char *recvbuf=(char *)malloc(sizeof(char)*(size+5));
		      			recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
				}
				if(!breakflag)
				{
		      			//res = pwrite(fd, "\n", strlen("\n"), offset);
					//tot += res;
					printf("total is %d\n",tot);
				      	sprintf(tcp_buf,"%d",tot);					
					send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
				}
		      		close(fd);
		    	}
		  	else
		    	{
				memset(tcp_buf,0,MAXLEN);
				sprintf(tcp_buf,"ERROR\n%d\n",-errno);
		    		send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
		    	}
			/*memset(tcp_buf,0,MAXLEN);
		      	printf("here4");fflush(stdout);
		      	memset(tcp_buf,0,MAXLEN);
		      	recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		   	memset(tcp_buf,0,MAXLEN);
		   	sprintf(tcp_buf,"%d",tot);
		  	send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);*/
		      	memset(tcp_buf,0,MAXLEN);
		      	printf("End of write\n");
			free(path);
                }
		else if(strcmp(a,"GETDIR") == 0)
                {
                        struct stat stbuf;
			int res;
			printf("Received message is %s\n",a);
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
		free(path);
		 closedir(dp);
                }
               else if(strcmp(a,"ACCESS") == 0)
                {
			char *path;
			int mask;
			int res;
                        printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);
			printf("Path is %s\n",path);

			a = strtok(NULL,"\n");
			mask = atoi(a);
			printf("Mask is %d\n",mask);
			
			memset(tcp_buf,0,MAXLEN);
		        res = access(path, mask);
		        if (res == -1)
                		sprintf(tcp_buf,"FAIL\n%d\n",-errno);	
			else
				sprintf(tcp_buf,"SUCCESS\n%d\n",0);
			printf("Sent %s",tcp_buf);
			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			free(path);
                }
               else if(strcmp(a,"CHMOD") == 0)
                {
			int res;
                        mode_t mode;

			printf("Received message is %s\n",a);

			a = strtok(NULL,"\n");
                        path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
                        strcpy(path,rootpath);
                        strcat(path,a);
                        printf("Path is %s\n",path);

                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
                        printf("Sent %s",tcp_buf);
                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

                        recv(clientList[n].conn_socket,(char *)&mode,sizeof(mode_t),0);
                        printf("Received mode\n");
			fflush(stdout);
                        res = chmod(path, mode);

                        memset(tcp_buf,0,MAXLEN);
                        if (res == -1)
                                sprintf(tcp_buf,"FAIL\n%d\n",-errno);
                        else
                                sprintf(tcp_buf,"SUCCESS\n%d\n",0);

                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			free(path);
			memset(tcp_buf,0,MAXLEN);
			printf("Chmod completed\n");
			fflush(stdout);
                }
               else if(strcmp(a,"CHOWN") == 0)
                {
			int res;
			uid_t uid;
			gid_t gid;

			printf("Received message is %s\n",a);

                        a = strtok(NULL,"\n");
                        path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
                        strcpy(path,rootpath);
                        strcat(path,a);
                        printf("Path is %s\n",path);

			memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
                        printf("Sent %s",tcp_buf);
                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

                        recv(clientList[n].conn_socket,(char *)&uid,sizeof(uid_t),0);
                        printf("Received uid\n");

			memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
                        printf("Sent %s",tcp_buf);
                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

                        recv(clientList[n].conn_socket,(char *)&gid,sizeof(gid_t),0);
                        printf("Received gid\n");

			res = lchown(path, uid, gid);

			memset(tcp_buf,0,MAXLEN);
                        if (res == -1)
                                sprintf(tcp_buf,"FAIL\n%d\n",-errno);
                        else
                                sprintf(tcp_buf,"SUCCESS\n%d\n",0);

                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			printf("chown end\n");
			free(path);
                }
               else if(strcmp(a,"RMDIR") == 0)
                {
                        char *path;
                        int res;

                        printf("Received message is %s\n",a);
                        a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
                        strcpy(path,rootpath);
                        strcat(path,a);

                        res = rmdir(path);
                        memset(tcp_buf,0,MAXLEN);
                        if (res == -1)
                                sprintf(tcp_buf,"FAIL\n%d\n",-errno);
                        else
                                sprintf(tcp_buf,"SUCCESS\n%d\n",0);

                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
                        memset(tcp_buf,0,MAXLEN);
                        printf("Sent %s\n",tcp_buf);
			printf("rmdir ends\n");
                        free(path);
                }
               else if(strcmp(a,"RENAME") == 0)
                {
			char *from;
			char *to;
			int res;	
		
                        printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			from = (char*)malloc(strlen(rootpath)+strlen(a)+5);
                        strcpy(from,rootpath);
                        strcat(from,a);
			printf("from is\n%s\n",from);
			a = strtok(NULL,"\n");
			to = (char*)malloc(strlen(rootpath)+strlen(a)+5);
                        strcpy(to,rootpath);
                        strcat(to,a);
			printf("to is\n%s\n",to);

			res = rename(from, to);
			memset(tcp_buf,0,MAXLEN);
                        if (res == -1)
				sprintf(tcp_buf,"FAIL\n%d\n",-errno);
                        else
				sprintf(tcp_buf,"SUCCESS\n%d\n",0);

                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			printf("Sent %s",tcp_buf);
			free(from);
			free(to);
			printf("rename ends\n");
                }
               else if(strcmp(a,"SYMLINK") == 0)
                {
			char *from;
			char *to;
			int res;

			printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			from = (char*)malloc(strlen(rootpath)+strlen(a)+5);
//			strcpy(from,rootpath);
//			strcat(from,"/");
			strcat(from,a);
			printf("from is\n%s\n",from);
			a = strtok(NULL,"\n");
			to = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(to,rootpath);
			strcat(to,a);
			printf("to is\n%s\n",to);

                        res = symlink(from, to);
			memset(tcp_buf,0,MAXLEN);
			if (res == -1)
				sprintf(tcp_buf,"FAIL\n%d\n",-errno);
			else
				sprintf(tcp_buf,"SUCCESS\n%d\n",0);

			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			printf("Sent %s",tcp_buf);
			free(from);
			free(to);
			printf("symlink ends\n");	
                }
		else if(strcmp(a,"LINK") == 0)
                {

			char *from;
			char *to;
			int res;

			printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			from = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(from,rootpath);
			strcat(from,a);
			printf("from is\n%s\n",from);
			a = strtok(NULL,"\n");
			to = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(to,rootpath);
			strcat(to,a);
			printf("to is\n%s\n",to);

			res = link(from, to);
			memset(tcp_buf,0,MAXLEN);
			if (res == -1)
				sprintf(tcp_buf,"FAIL\n%d\n",-errno);
			else
				sprintf(tcp_buf,"SUCCESS\n%d\n",0);

			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			printf("Sent %s",tcp_buf);
			free(from);
			free(to);
			printf("link ends\n");
                }
               	else if(strcmp(a,"UNLINK") == 0)
                {
			char *path;
			int res;

			printf("Received message is %s\n",a);
			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);

                        res = unlink(path);
			memset(tcp_buf,0,MAXLEN);
			if (res == -1)
				sprintf(tcp_buf,"FAIL\n%d\n",-errno);
			else
				sprintf(tcp_buf,"SUCCESS\n%d\n",0);

			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			printf("Sent %s",tcp_buf);
			free(path);
			printf("unlink ends\n");
                }
		else if(strcmp(a,"READLINK") == 0)
		{
			int res;
			char *path;
			char *buf;
			size_t size;
		 	
			printf("Received message is %s\n",a);

                        a = strtok(NULL,"\n");
                        path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
                        strcpy(path,rootpath);
                        strcat(path,a);
                        printf("Path is %s\n",path);

			memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
                        printf("Sent %s",tcp_buf);
                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

                        recv(clientList[n].conn_socket,(char *)&size,sizeof(size_t),0);
                        printf("Received size\n");
			buf = (char*)malloc(sizeof(char)*(size-1));
			res = readlink(path, buf, size - 1);
			memset(tcp_buf,0,MAXLEN);
			if (res == -1)
				sprintf(tcp_buf,"FAIL\n%d\n",-errno);
                        else
			{
				buf[res] = '\0';
				sprintf(tcp_buf,"SUCCESS\n%s\n%d\n",buf,0);
			}

			send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			printf("Sent %s\n",tcp_buf);
			free(path);
			printf("readlink ends\n");

		}
		else if(strcmp(a,"UTIME") == 0)
		{
			int res;
		        struct timeval tv[2];
			struct timespec ts[2];
			printf("Received message is %s\n",a);

			a = strtok(NULL,"\n");
                        path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
                        strcpy(path,rootpath);
                        strcat(path,a);
                        printf("Path is %s\n",path);

			memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
                        printf("Sent %s",tcp_buf);
                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

                        recv(clientList[n].conn_socket,(char*)&ts[0],sizeof(struct timespec),0);
                        printf("Received ts[0]\n");

			memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
                        printf("Sent %s",tcp_buf);
                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

                        recv(clientList[n].conn_socket,(char*)&ts[1],sizeof(struct timespec),0);
                        printf("Received ts[1]\n");

			tv[0].tv_sec = ts[0].tv_sec;
			tv[0].tv_usec = ts[0].tv_nsec / 1000;
			tv[1].tv_sec = ts[1].tv_sec;
			tv[1].tv_usec = ts[1].tv_nsec / 1000;

			res = utimes(path, tv);

			memset(tcp_buf,0,MAXLEN);
                        if (res == -1)
                                sprintf(tcp_buf,"FAIL\n%d\n",-errno);
                        else
                                sprintf(tcp_buf,"SUCCESS\n%d\n",0);

                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			free(path);
		}
		else if(strcmp(a,"SETXATTR") == 0)
		{
			char *name;
			char *value;
			int flags;
			size_t size;
			printf("Received message is %s\n",a);

			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);
                        printf("Path is %s\n",path);

			a = strtok(NULL,"\n");
			name = (char*)malloc(strlen(a)+1);
			strcpy(name,a);
                        printf("name is %s\n",name);

			a = strtok(NULL,"\n");
			value = (char*)malloc(strlen(a)+1);
			strcpy(value,a);
                        printf("value is %s\n",value);

			a = strtok(NULL,"\n");
			flags = atoi(a);

                        memset(tcp_buf,0,MAXLEN);
                        strcpy(tcp_buf,"ACK\n");
                        printf("Sent %s",tcp_buf);
                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);

                        recv(clientList[n].conn_socket,(char *)&size,sizeof(size_t),0);
                        printf("Received size\n");
			fflush(stdout);
                        
			int res = lsetxattr(path, name, value, size, flags);

			memset(tcp_buf,0,MAXLEN);
                        if (res == -1)
                                sprintf(tcp_buf,"FAIL\n%d\n",-errno);
                        else
                                sprintf(tcp_buf,"SUCCESS\n%d\n",0);

                        send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			memset(tcp_buf,0,MAXLEN);
			free(path);
			free(name);
			free(value);
		}
		else if(strcmp(a,"GETXATTR") == 0)
                {
			char *name;
			char *value;
			int flags;
			size_t size;
			printf("Received message is %s\n",a);

			a = strtok(NULL,"\n");
			path = (char*)malloc(strlen(rootpath)+strlen(a)+5);
			strcpy(path,rootpath);
			strcat(path,a);
                        printf("Path is %s\n",path);

			a = strtok(NULL,"\n");
			name = (char*)malloc(strlen(a)+1);
			strcpy(name,a);
                        printf("name is %s\n",name);
			memset(tcp_buf,0,MAXLEN);

			int res = lgetxattr(path, name, value, size);
                        if (res == -1)
			{
				sprintf(tcp_buf,"FAIL\n%d\n",-errno);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			}
                        else
			{
				sprintf(tcp_buf,"SUCCESS\n%d\n%s\n",0,value);
				send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
				recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
				send(clientList[n].conn_socket,(char*)&size,sizeof(size_t),0);
			}

			memset(tcp_buf,0,MAXLEN);
			free(path);
			free(name);
		}
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

