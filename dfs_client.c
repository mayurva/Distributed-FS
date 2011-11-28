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

//	struct stat *stbuf;
        FILE *fp;
        char *fname;

	printf("Inside getattr Path is: %s\n",path);
	memset(tcp_buf,0,MAXLEN);
	sprintf(tcp_buf,"GETATTR\n%s\n",path);
	
	//res=lstat(path,stbuf);

//tcp code goes here
	send(sock,tcp_buf,strlen(tcp_buf),0);
	memset(tcp_buf,0,MAXLEN);
	recv(sock,(char *)stbuf,sizeof(struct stat),0);
	printf("Received stbuf\n");	
	
//rest of the code goes here

	res = lstat(path,stbuf);
        fname=(char *)malloc(sizeof(char)*(strlen(path)+7));
         
        strcpy(fname,".");
        strcat(fname,path);
        strcat(fname,".attr");

        fp=fopen(fname,"wb");
        fwrite(stbuf,1,sizeof(struct stat),fp);
	fclose(fp);

	return 0;
}

static int dfs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	printf("Inside mknod Path is: %s\n",path);

//client side code goes here
        memset(tcp_buf,0,MAXLEN);
	strcpy(tcp_buf,"MKNOD\n");	

//tcp code goes here
	send(sock,tcp_buf,strlen(tcp_buf),0);
	memset(tcp_buf,0,MAXLEN);
	recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
	printf("Received message: %s\n",tcp_buf);	

	/*fd = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
	if(fd >= 0)
		fd = close(fd);
	if(fd == -1)
		return -errno*/;
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
        strcpy(tcp_buf,"GETDIR\n");

//tcp code goes here
        send(sock,tcp_buf,strlen(tcp_buf),0);
        memset(tcp_buf,0,MAXLEN);
        recv(sock,tcp_buf,MAXLEN,0);

//rest of the code goes here
        printf("Received message: %s\n",tcp_buf);
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
