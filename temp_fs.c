#include<fuse.h>
#include<stdio.h>
#include <string.h>
#include <fcntl.h>
#include<errno.h>

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
 
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &dfs_oper);	
}
