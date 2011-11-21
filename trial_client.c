#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]) {
  FILE * pFile;
  long lSize;
  char * buffer,fileSize[64],tempAck[64];
  size_t result;
  int lenFlag;
  int serverSocket,i,connectFlag;
  long lsize;
  char hostName[256],hostIP[64];
  struct hostent *hostAttr;
  struct sockaddr_in hostAddr;
  if(argc==2)
    {
    printf("Success");
    fflush(stdout);
    }
    
  strcpy(hostName,argv[1]);


  serverSocket=createSocket();
  //gethostname(hostName,sizeof(hostName));

  hostAttr=gethostbyname(hostName);
      if(hostAttr==NULL)
	{
	  printf("\n\nHost not found\n");
	  exit(1);
	}

   hostAddr.sin_family=AF_INET;
   hostAddr.sin_port=htons(5000);
   memcpy(&hostAddr.sin_addr,hostAttr->h_addr_list[0],hostAttr->h_length);



   connectFlag=connect(serverSocket,(struct sockaddr *)&hostAddr,sizeof(hostAddr));
   if(connectFlag<0)
     {
       printf("\nError in connect\n");
       exit(1);
     }



  pFile = fopen ( "temp_fs.c" , "ab+" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  //  printf("%s",buffer);

  sprintf(fileSize,"%ld",lsize);
  send(serverSocket,fileSize,strlen(fileSize),0);
  lenFlag=recv(serverSocket,tempAck,strlen(tempAck),0);
  send(serverSocket,buffer,strlen(buffer),0);
  /* the whole file is now loaded in the memory buffer. */

  // terminate
  fclose (pFile);
  free (buffer);
  return 0;
}
