INC = dfs.h
OBJ = client_util.o  dfs_client.o  dfs_server.o  server_util.o  tcp_utils.o
CLI_SRC = client_util.c dfs_client.c tcp_utils.c
SER_SRC = dfs_server.c server_util.c tcp_utils.c
CLI_OUT = client
SER_OUT = server_dir/server
CC = cc

FLAGS = `pkg-config fuse --cflags --libs`


all: client server

temp:
	$(CC) -g $(FLAGS) temp_fs.c -o temp_fs

client:
	$(CC) -g $(FLAGS) $(CLI_SRC) -o $(CLI_OUT)

server:
	$(CC) -g $(FLAGS) $(SER_SRC) -o $(SER_OUT)
	
.PHONY: clean 
clean:
	rm -f $(CLI_OUT) $(SER_OUT)


