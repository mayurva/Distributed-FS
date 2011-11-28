#include<fuse.h>
#include<stdio.h>
#include <string.h>
#include <fcntl.h>
#include<errno.h>
#include"dfs.h"

char tcp_buf[MAXLEN];

//below is implementation of all FS functions. They should be split in client and server
static int dfs_getattr(const char *path, struct stat *stbuf)
{
	int res;
//client side code goes here

	struct stat temp_stbuf;
        FILE *fp;
        char *fname;

	printf("Inside getattr Path is: %s\n",path);
	memset(tcp_buf,0,MAXLEN);
	sprintf(tcp_buf,"GETATTR\n%s\n",path);
	
	//res=lstat(path,stbuf);

//tcp code goes here
	send(sock,tcp_buf,strlen(tcp_buf),0);
	memset(tcp_buf,0,MAXLEN);
	recv(sock,(char *)&temp_stbuf,sizeof(struct stat),0);
	printf("Received stbuf\n");	
	
//rest of the code goes here

/*	stbuf = (struct stat*) malloc(sizeof(struct stat));
	stbuf->st_dev = temp_stbuf.st_dev;
	stbuf->st_ino = temp_stbuf.st_ino;
	stbuf->st_mode = temp_stbuf.st_mode;
	stbuf->st_nlink = temp_stbuf.st_nlink;
	stbuf->st_uid = temp_stbuf.st_uid;
	stbuf->st_gid = temp_stbuf.st_gid;
	stbuf->st_rdev = temp_stbuf.st_rdev;
	stbuf->st_size = temp_stbuf.st_size;
	stbuf->st_blksize = temp_stbuf.st_blksize;
	stbuf->st_blocks = temp_stbuf.st_blocks;
	stbuf->st_atime = temp_stbuf.st_atime;
	stbuf->st_mtime = temp_stbuf.st_mtime;
	stbuf->st_ctime = temp_stbuf.st_ctime;
*/
//	printf("User id %d\n",stbuf->st_uid);
//	char rootpath[1000] = "/home/mayur/Git Hub/Distributed-FS";
//	if(strcmp(path,"/") == 0)
//		strcpy(rootpath,"/home/mayur/Git Hub/Distributed-FS/tempfs");

	res = lstat(path,stbuf);

//	printf("User id %d\n",stbuf->st_uid);

        //fname=(char *)malloc(sizeof(char)*(strlen(path)+7));
        //strcpy(fname,".");
        //strcat(fname,path);
        //strcat(fname,".attr");

        //fp=fopen(fname,"wb");
        //fwrite(&temp_stbuf,1,sizeof(struct stat),fp);
	//fclose(fp);

//	printf("End of getattr\n");
	if(res == -1)
		return -errno;
	return 0;
}

static int dfs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
	printf("Inside mknod Path is: %s\n",path);

//client side code goes here

        memset(tcp_buf,0,MAXLEN);
	sprintf(tcp_buf,"MKNOD\n%s",path);	


//tcp code goes here
	send(sock,tcp_buf,strlen(tcp_buf),0);
	recv(sock,tcp_buf,MAXLEN,0);

	send(sock,(char*)&mode,strlen(tcp_buf),0);
	recv(sock,tcp_buf,MAXLEN,0);

	send(sock,(char*)&rdev,strlen(tcp_buf),0);

	memset(tcp_buf,0,MAXLEN);
	recv(sock,tcp_buf,MAXLEN,0);
	printf("Received message: %s\n",tcp_buf);	
	sscanf(tcp_buf,"%d",&res);

//rest of the code goes here
	if (res == -1)
		return -errno;

	return 0;
}

static int dfs_mkdir(const char *path, mode_t mode)
{
//client side code goes here
        printf("Inside mknod Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"MKNOD\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);
//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_open(const char *path, struct fuse_file_info *fi)
{
//client side code goes here
        printf("Inside open Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"OPEN\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
//client side code goes here
        printf("Inside read Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"READ\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_write(const char *path, const char *buf, size_t size,off_t offset, struct fuse_file_info *fi)
{
//client side code goes here
        printf("Inside write Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"WRITE\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);
//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}


static int dfs_getdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
{
//client side code goes here
        printf("Inside getdir Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
    sprintf(tcp_buf,"GETDIR\n%s\n",path);


//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        int recFlag=recv(sock,tcp_buf,MAXLEN,0);
    if(recFlag<0){
      printf("Error while receiving");
      exit(1);
    }

        send(sock,"ok",strlen("ok"),0);
    int flag=1;
    while(1)
      {
        struct stat tempSt;
        recv(sock,(char*)&tempSt,sizeof(struct stat),0);
            send(sock,"ok",strlen("ok"),0);
        int recFlag=recv(sock,tcp_buf,MAXLEN,0);
        if(recFlag<0){
          printf("Error while receiving");
          exit(1);
        }
        tcp_buf[recFlag]='\0';
	if(strcmp(tcp_buf,"end")==0)
	  break;
        if (filler(buf, tcp_buf, &tempSt, 0))
          {
	    flag=0;
	    break;
          }
      }
//rest of the code goes here
    //  printf("Received message: %s\n",tcp_buf);
        return 0;
}


static int dfs_access(const char *path, int mask)
{
//client side code goes here
        printf("Inside access Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"ACCESS\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_chmod(const char *path, mode_t mode)
{
//client side code goes here
        printf("Inside chmod Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"CHMOD\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_chown(const char *path, uid_t uid, gid_t gid)
{
//client side code goes here
        printf("Inside chown Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"CHOWN\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_rmdir(const char *path)
{
//client side code goes here
        printf("Inside rmdir Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"RMDIR\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_rename(const char *from, const char *to)
{
//client side code goes here
        printf("Inside rename Path is: %s\n",from);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"RENAME\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_symlink(const char *from, const char *to)
{
//client side code goes here
        printf("Inside symlink Path is: %s\n",from);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"SYMLINK\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_link(const char *from, const char *to)
{
//client side code goes here
        printf("Inside link Path is: %s\n",from);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"LINK\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

static int dfs_unlink(const char *path)
{
//client side code goes here
        printf("Inside unlink Path is: %s\n",path);
        memset(tcp_buf,0,MAXLEN);
        strcpy(tcp_buf,"UNLINK\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
        return 0;
}

//below is client code
static struct fuse_operations dfs_oper = {
.getattr = (void *)dfs_getattr,
.mknod = (void *)dfs_mknod,
.mkdir = (void *)dfs_mkdir,
.open = (void *)dfs_open,
.read = (void *)dfs_read,
.write = (void *)dfs_write,
.getdir = (void *)dfs_getdir,
// .access = (void *)dfs_access,
.chmod = (void *)dfs_chmod,
.chown = (void *)dfs_chown,
.rmdir = (void *)dfs_rmdir,
.rename = (void *)dfs_rename,
.symlink = (void *)dfs_symlink,
.link = (void *)dfs_link,
.unlink = (void *)dfs_unlink,
 
};

int main(int argc, char *argv[])
{
	int i;
	initClient(argc,argv);
	for(i=1;i<argc;i++)
		argv[i] = argv[i+1];
	argc--;
	umask(0);
	return fuse_main(argc, argv, &dfs_oper);	
}
