#include<fuse.h>
#include<stdio.h>
#include <string.h>
#include <fcntl.h>

static int dfsClient_open(const char *path, struct fuse_file_info *fi)
{
	printf("Hello World\n");
	return 0;
}

static int dfsClient_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	printf("Hello World\n");
/*	printf("Hello World\n");
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, NULL, NULL, 0);
*/
	return 0;
}

static struct fuse_operations dfsClient_oper = {
	.getdir		= (void*)dfsClient_readdir,
	.open		= (void*)dfsClient_open,
};

int main(int argc, char *argv[])
{
	umask(0);
	initClient(argc,argv);
	for(i=2;i<argc;i++)
		argv[i] = argv[i-1];
	argc--;
	return fuse_main(argc, argv, &dfsClient_oper);	
}
