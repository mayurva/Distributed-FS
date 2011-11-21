#include<fuse.h>
#include<stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
int serverSocket;

//below is implementation of all FS functions. They should be split in client and server
static int dfs_getattr(const char *path, struct stat *stbuf)
{
	int res;
	//client side code goes here
	printf("Inside getattr Path is: %s\n",path);
	
	//tcp code goes here

	//server code goes here
	if((res = lstat(path, stbuf)) == -1)
		return -errno;
	return 0;

}

static int dfs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int fd;
	printf("Inside mknod Path is: %s\n",path);
	//client side code goes here

	//tcp code goes here

	//server code goes here
	fd = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
	if(fd >= 0)
		fd = close(fd);
	if(fd == -1)
		return -errno;
	return 0;
}

static int dfs_mkdir(const char *path, mode_t mode)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_open(const char *path, struct fuse_file_info *fi)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_write(const char *path, const char *buf, size_t size,off_t offset, struct fuse_file_info *fi)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_getdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_access(const char *path, int mask)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_chmod(const char *path, mode_t mode)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_chown(const char *path, uid_t uid, gid_t gid)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_rmdir(const char *path)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_rename(const char *from, const char *to)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_symlink(const char *from, const char *to)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_link(const char *from, const char *to)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

static int dfs_unlink(const char *path)
{
	//client side code goes here

	//tcp code goes here

	//server code goes here

}

/*
//below is client code
static struct fuse_operations dfs_oper = {
	.getattr	= (void *)dfs_getattr,
	.mknod 		= (void *)dfs_mknod,
	.mkdir		= (void *)dfs_mkdir,
	.open		= (void *)dfs_open,
	.read		= (void *)dfs_read,
	.write		= (void *)dfs_write,
	.getdir		= (void *)dfs_getdir,
//	.access		= (void *)dfs_access,
	.chmod		= (void *)dfs_chmod,
	.chown		= (void *)dfs_chown,
	.rmdir		= (void *)dfs_rmdir,
	.rename		= (void *)dfs_rename,
	.symlink	= (void *)dfs_symlink,
	.link		= (void *)dfs_link,
	.unlink		= (void *)dfs_unlink,
 
};*/


void waitForClientMessage(void *ptr)
{
  int lenFlag;
  FILE *tempFP;
  char cmd[64],*p,*filePtr,*buffer;
  long fileSize;
  //  while(1)
  //  {
      lenFlag=recv((int)ptr,cmd,63,0);
      if(lenFlag<0)
	{
	  printf("\nReceive Error\n");
	  exit(1);
	}
      cmd[lenFlag]='\0';
      fileSize=strtol(cmd,&p,10);
      send((int)ptr,"next",strlen("next"),0);
      filePtr=(char *)malloc(sizeof(char)*(fileSize+1));
      printf("%ld",fileSize);
      fflush(stdout);
      lenFlag=recv((int)ptr,filePtr,fileSize,0);
      if(lenFlag<0)
	{
	  printf("\nReceive Error\n");
	  exit(1);
	}
      filePtr[lenFlag]='\0';
      printf("Success");
    fflush(stdout);

      tempFP=fopen(".cmd.tmp","wb");
      fwrite(filePtr,1,fileSize+1,tempFP);
       fclose(tempFP);
      printf("Success");
    fflush(stdout);
    tempFP=fopen(".cmd.tmp","rb");
      buffer = (char*) malloc (sizeof(char)*fileSize+1);
      if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

      // copy the file into the buffer:
      fread (buffer,1,fileSize,tempFP);
      //if (result != Size) {fputs ("Reading error",stderr); exit (3);}

      printf("%s",buffer);
      //  }
  
  
}

int main(int argc, char *argv[])
{
  //umask(0);
  int clientSocket,serverPort,i;
  char hostName[256],hostIP[64];
  struct hostent *hostAttr;
  struct sockaddr_in hostAddr;
  pthread_t threads[10];
  serverSocket=createSocket();
  gethostname(hostName,sizeof(hostName));
  printf("%s",hostName);
  fflush(stdout);
  hostAttr=gethostbyname(hostName);
      if(hostAttr==NULL)
	{
	  printf("\n\nHost not found\n");
	  exit(1);
	}
      serverPort=bindSocket(serverSocket,5000,hostAttr);
      
      listenSocket(serverSocket);


      i=0;
      // while(1)
	{
	  clientSocket=acceptConnection(serverSocket);
	  pthread_create(&threads[i],NULL,waitForClientMessage,(void *)clientSocket);
	
	  pthread_join(threads[i],NULL);  
	}
	//return fuse_main(argc, argv, &dfs_oper);	
}
