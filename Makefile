INC = 
OBJ = client.o
SRC = client.c
OUT = server client
CC = cc
#FLAGS = `pkg-config fuse --cflags --libs'

all:
	cc -g `pkg-config fuse --cflags --libs` client.c -o client

temp:
	cc -g `pkg-config fuse --cflags --libs` temp_fs.c -o temp_fs

#all: $(OBJ) client server

#$(OBJ): $(INC)
	

client: 
	$(CC) -g $(FLAGS) cleint.c  -D_FILE_OFFSET_BITS=64

server: 
#	cc -o server server.o $(FLAGS)

.PHONY: clean 
clean:
	rm -f $(OUT) $(OBJ)


