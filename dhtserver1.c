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
#define PORT 21129
#define BUFSIZE 500
#define MAXBUF 1000
void paddr(unsigned char *a);
int main()
{
  FILE *fp;
  long lSize;
  char *buffer;
  char *ret;
  char reply1[500],post1[100];
  long i;
  int sa_len,recvlen,pos,j=0,k=0,loc,temp;
  int nread;
  int s;
  int sockUDP,sockTCP1,sockTCP2;
  int num=0;
  struct hostent *hp;
  struct sockaddr_in server_addr,server_TCP1,server_TCP2;
  socklen_t addrlen = sizeof(server_addr); 
  unsigned char buf[BUFSIZE];   
  char token[50];
  char get[100]="GET";
  char get1[50];
  char key1[8];
  char post[100]="POST ";
  char *host="nunki.usc.edu";
  char reply[MAXBUF];
  fp=fopen("server1.txt","r");
  if(fp==NULL)
	{
		perror("Error opening the file");
		return(-1);
	}
  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  rewind( fp );
  buffer = calloc( 1, 1024 );
  if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);
	/* copy the file into the buffer */
  if( 1!=fread( buffer , lSize, 1 , fp) )
	fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
	//printf("%s",buffer);
	
   //UDP socket for communicatng with the clients
  if ((sockUDP = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
     {
		perror("Socket could not be created !!");
		return 0;
	 }
     memset((char *)&server_addr, 0, sizeof(server_addr));
	 server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     server_addr.sin_port = htons(PORT);
	
	if (bind(sockUDP, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
	 {
		perror("bind failed");
		return 0;
	 }
	 
 /*
	if ((sockTCP1 = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	 {
		perror("Socket could not be created !!");
		return 0;
	 }
  
    sa_len = sizeof(server_TCP1);
	
	if (getsockname(sockTCP1,(struct sockaddr *) &server_TCP1, &sa_len) == -1) 
	 {
		perror("getsockname() failed");
        return -1;
     }  
     memset((char *)&server_TCP1, 0, sizeof(server_TCP1));
	 server_TCP1.sin_family = AF_INET;
     server_TCP1.sin_addr.s_addr = htonl(INADDR_ANY);
     server_TCP1.sin_port = htons(0);
	 
	 if (bind(sockTCP1, (struct sockaddr *)&server_TCP1, sizeof(server_TCP1)) < 0) 
	 {
		perror("bind failed");
		return 0;
     }
	 */
	//printf("\n waiting on port %d\n", PORT);
	
	 struct hostent *hp1;
     hp1 = gethostbyname(host); // code reused from Rutgers.edu
    if (!hp1) 
	  {
		fprintf(stderr, "could not obtain address of %s\n", host);
		return 0;
      }
	
    for (j=0; hp1->h_addr_list[j] != 0; j++)
        paddr((unsigned char*) hp1->h_addr_list[j]);
  
    for(;;){
	recvlen = recvfrom(sockUDP, buf, BUFSIZE, 0, (struct sockaddr *)&server_addr, &addrlen);
    if(recvlen > 0)
    {
		buf[recvlen]=0;
		num=num+1;            // incremented for differentiating between client 1 and client 2 
		sscanf(buf,"%s %s",get,token); // to get only the key 
		//printf("\n %s",token);
		if(num % 2 == 1)      // for client 1
			printf("\n The Server 1 has received a request with %s from client 1 with port number %d and I.P address %s",token,ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));
		else               //for client 2
			printf("\n The Server 1 has received a request with %s from client 2 with port number %d and I.P address %s",token,ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));	 
		ret = strstr(buffer,token);
		//printf("%s\n",buffer);
		//printf("%s\n",token);
		if(ret!=NULL)
		 {
			pos = ret -  buffer;
			//printf("\n%d",pos);
			while(buffer[pos]!=' ') // to remove the blank spaces between the key and value stored in the buffer
			{
				pos++;
			}
			pos=pos+1;
			//printf("pos==%d\n",pos);
			//printf("%c\n",buffer[pos]);
			j=0;
			for(j=0;j<7;j++)
			{
			  key1[j]=buffer[pos];
			  pos++;
			}
			key1[j]='\0';
			strcat(post,key1);
			if (num % 2 == 1)
				printf("\n The Server 1 sends the reply %s to the Client 1 with port number %d and IP address %s",post,ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));
			else
				printf("\n The Server 1 sends the reply %s to the Client 2 with port number %d and IP address %s",post,ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));   
			sendto(sockUDP,post,strlen(post),0 , (struct sockaddr *)&server_addr,addrlen);
			strcpy(post,"POST ");
   }
   
   else 
	{
	//printf("entering in else\n");
      strcat(get," ");
      strcat(get,token);
      //printf("\n%s",get);
      memset((char *)&server_TCP1, 0, sizeof(server_TCP1));  // reused from Rutgers and Beej's tutorial
	  server_TCP1.sin_family = AF_INET;
      server_TCP1.sin_addr.s_addr = htonl(INADDR_ANY);
	  server_TCP1.sin_port = htons(0);
	  if ((sockTCP1 = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	  {
		perror("Socket could not be created !!");
		return 0;
	  }
     if (bind(sockTCP1, (struct sockaddr *)&server_TCP1, sizeof(server_TCP1)) < 0) 
	 {
		perror("bind failed");
		return 0;
     }
	 sa_len = sizeof(server_TCP1);
	 if (getsockname(sockTCP1,(struct sockaddr *) &server_TCP1, &sa_len) == -1) 
	 {
		perror("getsockname() failed");
        return -1;
     }  
	 hp = gethostbyname(host);
   	 if (!hp) 
	  {
		printf("\ncould not obtain address of %s\n", host);
		return 0;
	  }
	  memcpy((void *)&server_TCP1.sin_addr, hp->h_addr_list[0], hp->h_length); // reused from Beej's tutorial
	  memset((char *)&server_TCP2,0,sizeof(server_TCP2));
	  server_TCP2.sin_family=AF_INET;
	  server_TCP2.sin_port=htons(22129);
	  memcpy((void *)&server_TCP2.sin_addr, hp->h_addr_list[0], hp->h_length);
	  
	  printf("\n The Server1 sends the request %s to the Server 2.\n The TCP Port number is %d and I.P address %s \n  ",get,ntohs(server_TCP1.sin_port),inet_ntoa(server_TCP1.sin_addr));		 
	  if (connect(sockTCP1, (struct sockaddr *)&server_TCP2, sizeof(server_TCP2)) < 0) 
	  {
	    perror("\nconnect failed");
	    return 0;
      }
      nread=write(sockTCP1,get,MAXBUF);
      //printf("\n DONE %s\n",get);
      strcpy(get,"GET");
      nread = read(sockTCP1,reply,MAXBUF);
      if(nread > 0)
     {
       sscanf(reply,"%s %s",post1,reply1);
	   printf("\n The Server 1 received the value %s from the Server 2 with the port number %d and IP address %s \n ",reply1,ntohs(server_TCP2.sin_port),inet_ntoa(server_TCP2.sin_addr));  
       strcat(buffer,"\n");
       strcat(buffer,token);
       strcat(buffer," ");
       strcat(buffer,reply1);
       //printf("%s",buffer);
     }
     close(sockTCP1);
     sendto(sockUDP,reply,strlen(reply),0 , (struct sockaddr *)&server_addr,addrlen);
	 if(num % 2 == 1)
     printf("\n The Server 1 sent the reply %s to the Client 1 with port number %d and IP address  %s ",reply,ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));
    else
		 printf("\n The Server 1 sent the reply %s to the Client 2 with port number %d and IP address  %s ",reply,ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));
    }
   }
 }
    return(0);
}

void paddr(unsigned char *a)
{
        printf("The Server 1 has UDP port number %d and I.P address %d.%d.%d.%d\n",PORT, a[0], a[1], a[2], a[3]);
}
