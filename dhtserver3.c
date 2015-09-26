#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define MAXBUF 512
#define BUFSIZE 400
#define SERVER3_PORT 23129

int main()
{
  FILE *fp3;
  char *buffer3;
  long lSize;
  int sockTCP3,nread=0,pos=0,rqst=0;
  struct sockaddr_in server_addr,server_TCP;
  socklen_t alen;
  char *ret;
  char token[50],get1[50]="",key1[50]="";
  char post[50]="";
  char inputbuf[BUFSIZE];
  int server3_port=23129,j=0;
  struct hostent *hp;
  char *host = "nunki.usc.edu";
  int i;
  fp3=fopen("server3.txt","r");
  if(fp3==NULL)
	{
	  perror("Error opening the file");
      return(-1);
    }
  fseek( fp3 , 0L , SEEK_END);
  lSize = ftell( fp3 );
  rewind( fp3 );
  buffer3 = calloc( 1, 1024 );   //buffer created dynamically
  if( !buffer3 ) fclose(fp3),fputs("memory alloc fails",stderr),exit(1);

  if( 1!=fread( buffer3 , lSize, 1 , fp3) )
   fclose(fp3),free(buffer3),fputs("entire read fails",stderr),exit(1);
  //printf("%s",buffer3);
  if ((sockTCP3 = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
  {
	perror("Socket could not be created !!");
	return 0;
  }
  memset((char *)&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
//  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(server3_port);
   hp = gethostbyname(host);
        if (!hp) 
		{
                fprintf(stderr, "could not obtain address of %s\n", host);
                return 0;
        }
    memcpy((void *)&server_addr.sin_addr,hp->h_addr_list[0],hp->h_length);
    printf("\n The Server 3 has TCP port number %d and IP address %s",ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));
    if (bind(sockTCP3, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
		perror("bind failed");
	    return 0;
	}
  //printf("\n THIRD PORT DONE");
    if(listen(sockTCP3,5)<0)
    {
		perror("Listen failed");
       exit(1);
	}
    // printf("\n Listening!!");
	alen = sizeof(server_TCP);
	memset((char *)&server_TCP,0,sizeof(server_TCP));
    for(;;)
	 {
		while ((rqst = accept(sockTCP3,(struct sockaddr *)&server_TCP, &alen) )< 0 ); // reused from Rutgers
	    nread = read(rqst,inputbuf,BUFSIZE);
        //printf("\n%s",inputbuf); 
		sscanf(inputbuf,"%s %s",get1,token);
		printf("\n The Server 3 has received a request with %s from the Server 2 with port number %d and IP address %s ",token,ntohs(server_TCP.sin_port),inet_ntoa(server_TCP.sin_addr));
	    if(nread > 0)   
		{
		  printf("\n%s",token);
		  ret = strstr(buffer3,token); // strstr is used to find the substring in a particular string
		  if(ret!=NULL)
			{
				pos = ret -  buffer3;
				//printf("\n%d",pos);
				while(buffer3[pos]!=' ')
				{
					pos++;
				}
				pos=pos+1;
				j=0;
				while(buffer3[pos]!='\n')
				{
				  key1[j]=buffer3[pos];
				  j++;
				  pos++;
				}
				// printf("\n%s",key1); 
				strcpy(post, "POST ");
				//printf("\n%s",post);
				strcat(post,key1);
				//printf("\n%s",post);
				printf("\n The Server 3 sent the reply %s to the Server 2 with port number %d and IP address %s ",post,ntohs(server_TCP.sin_port),inet_ntoa(server_TCP.sin_addr));
				write(rqst,post,BUFSIZE); 
				strcpy(post,"POST ");
			}
		}
	}
}
