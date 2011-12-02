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
		printf("Recvd %d\n%s\n",n,tcp_buf);
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
		  int res,fd,flags;
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
		  if (fd != -1)
		    {
		      send(clientList[n].conn_socket,"success",strlen("success"),0);
		      memset(tcp_buf,0,MAXLEN);
		      res=(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		      if(res<0){
			printf("\nError receiving flags");
			exit(1);
		      }
		      tcp_buf[res]='\0';

		      int offset=atoi(tcp_buf);
		      memset(tcp_buf,0,MAXLEN);
		      strcpy(tcp_buf,"ok");
		      printf("here2");fflush(stdout);		      
		      while(strcmp(tcp_buf,"ok")==0)
			{
			  printf("here3");fflush(stdout);
			  memset(tcp_buf,0,MAXLEN);
			  int rflag=pread(fd,tcp_buf,BLOCKSIZE,offset);
			  printf("%s",tcp_buf);fflush(stdout);
			  if(rflag==-1)
			    {
			      send(clientList[n].conn_socket,"file_read_error",strlen("file_read_error"),0);
			    }
			  else
			    {
			      send(clientList[n].conn_socket,tcp_buf,strlen(tcp_buf),0);
			    }
			  

			  memset(tcp_buf,0,MAXLEN);
			  res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
			  if(res<0){
			    printf("\nError receiving flags");
			    exit(1);
			  }
			  tcp_buf[res]='\0';
			  printf("%s",tcp_buf);fflush(stdout);
			  offset=offset+BLOCKSIZE;
			}
		      printf("here6");fflush(stdout);
		      memset(tcp_buf,0,MAXLEN);
		      //sleep(10);
		    }
		  else
		    {
		      //printf("here5");fflush(stdout);
		    send(clientList[n].conn_socket,"failed",strlen("failed"),0);
		    }
		      printf("here4");fflush(stdout);
		  close(res);
			printf("End of read\n");

                }
		else if(strcmp(a,"WRITE") == 0)
                {
		  int res,fd,flags;
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
		      send(clientList[n].conn_socket,"success",strlen("success"),0);
		      memset(tcp_buf,0,MAXLEN);
		      res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		      if(res<0){
			printf("\nError receiving flags");
			exit(1);
		      }
		      tcp_buf[res]='\0';

		      int offset=atoi(tcp_buf);
		      send(clientList[n].conn_socket,"success",strlen("success"),0);
		      memset(tcp_buf,0,MAXLEN);
		      res=recv(clientList[n].conn_socket,tcp_buf,MAXLEN,0);
		      if(res<0){
			printf("\nError receiving flags");
			exit(1);
		      }
		      tcp_buf[res]='\0';

		      size_t size= (size_t)atoi(tcp_buf);
		      char *recvbuf=(char *)malloc(sizeof(char)*(int)size);
		      send(clientList[n].conn_socket,"success",strlen("success"),0);
		      memset(tcp_buf,0,MAXLEN);
		      res=recv(clientList[n].conn_socket,recvbuf,(int)size,0);
		      if(res<0){
			printf("\nError receiving flags");
			exit(1);
		      }
		      recvbuf[res]='\0';

		      //		      int offset=atoi(tcp_buf);
		      res = pwrite(fd, recvbuf, size, offset);
		      if (res == -1)
			 send(clientList[n].conn_socket,"failed",strlen("failed"),0);
		      else
			 send(clientList[n].conn_socket,"written",strlen("written"),0);
		    }
		  else
		    {
		      //printf("here5");fflush(stdout);
		    send(clientList[n].conn_socket,"failed",strlen("failed"),0);
		    }
		      printf("here4");fflush(stdout);
		      close(res);
		      printf("End of write\n");

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

