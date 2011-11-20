#include<fuse.h>
#include<stdio.h>
#include <string.h>
#include <fcntl.h>

static int dfsClient_open(const char *path, struct fuse_file_info *fi)
{
	printf("\nin open\n");
	return 0;
}

static int dfsClient_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	printf("\nIn readdir\n");
	/*	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, NULL, NULL, 0);*/

	return 0;
}
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res=0;

	printf("\nIn read\n");
	/*	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

		close(fd);*/

	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res=0;

	printf("\nIn write\n");

	/*	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);*/
	return res;
}



static struct fuse_operations dfsClient_oper = {
	.getdir	= dfsClient_readdir,
	.open		= dfsClient_open,
	.read		= xmp_read,
	.write		= xmp_write,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &dfsClient_oper);	
}
