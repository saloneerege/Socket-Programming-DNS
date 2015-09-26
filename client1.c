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
#define BUFSIZE 2048
int main()
{
  FILE *fp2;
  long lSize;
  char *buffc2;
  long i;
  int sa_len;
  int s;
  int client2_sock,j=0,k=0,d=0;
  char input[50],key[50]=" ";
  unsigned char answer[BUFSIZE]=" ";
  struct sockaddr_in client2_addr,server_addr;
  socklen_t addrlen = sizeof(server_addr);
   char key1[10]="GET ";
   char post[10],answer1[50];
   
  char *ret;
  int position=0;
   struct hostent *hp;
   char *host = "nunki.usc.edu";
   
    fp2=fopen("client1.txt","r");
  if(fp2==NULL)
  {
   perror("Error opening the file");
   return(-1);
  }
  fseek( fp2 , 0L , SEEK_END);
  lSize = ftell( fp2 );
  rewind( fp2 );
  
  /* allocate memory for entire content */
  buffc2 = calloc( 1, lSize+1 );
  if( !buffc2 ) fclose(fp2),fputs("memory alloc fails",stderr),exit(1);

  /* copy the file into the buffer */
  if( 1!=fread( buffc2 , lSize, 1 , fp2) )
  fclose(fp2),free(buffc2),fputs("entire read fails",stderr),exit(1);
  
  /*for(i=0;i<lSize+1;i++) 
      
    {
      printf("%c",buffc2[i]);
    }*/
    
  if ((client2_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	perror("Socket could not be created !!");
	return 0;
}
    
    memset((char *)&client2_addr, 0, sizeof(client2_addr));
    client2_addr.sin_family = AF_INET;
    client2_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client2_addr.sin_port = htons(0);
	hp = gethostbyname(host);
 if (!hp) {
     printf("could not obtain address of %s\n", host);
	return 0;
}

memcpy((void *)&client2_addr.sin_addr, hp->h_addr_list[0], hp->h_length);
if (bind(client2_sock, (struct sockaddr *)&client2_addr, sizeof(client2_addr)) < 0) {
	perror("bind failed");
	return 0;
}
    
    
  printf("\nPlease enter your search(USC,UCLA,etc) : ");
  scanf("%s",input);
  
  ret=strstr(buffc2,input);
  position = ret - buffc2 ;
 // printf("\n%d\n",position);
   while(buffc2[position]!=' ')
    position++;
   position=position+1;
   while(buffc2[position]!='\n')
   {
      key[j]=buffc2[position];
      j++;
      position++;
   }
   // printf("\n%s\n",key);
    printf("\n The Client 1 has received a request with the search word %s, which maps to key %s",input,key);
    strcat(key1,key);
   // printf("\n%s\n",key1);
  
 memset((char*)&server_addr, 0, sizeof(server_addr));
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(PORT);


 

 sa_len = sizeof(client2_addr);
 if (getsockname(client2_sock,(struct sockaddr *) &client2_addr, &sa_len) == -1) {
      perror("getsockname() failed");
      return -1;
   }  

printf("\n The Client 1's Port number is %d and IP address is %s",ntohs(client2_addr.sin_port),inet_ntoa(client2_addr.sin_addr));

memcpy((void *)&server_addr.sin_addr, hp->h_addr_list[0], hp->h_length);

printf("\n The Client 1 sends the request %s to the Server 1 with port number %d and IP address %s",key1,ntohs(server_addr.sin_port),inet_ntoa(server_addr.sin_addr));
if (sendto(client2_sock, key1, strlen(key1), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
	perror("sendto failed");
 }
 if(recvfrom(client2_sock,answer,BUFSIZE,0,(struct sockaddr*)&server_addr ,&server_addr) < 0){
     perror("receievd from failed");

 }
   else 
      {
        sscanf(answer,"%s %s",post,answer1);
        printf("\n The Client 1 received the POST %s from the Server 1 with port number %d and IP address %s  \n",answer1,PORT,inet_ntoa(server_addr.sin_addr));
		printf("\n The Client 1's port number is %d and IP address %s",ntohs(client2_addr.sin_port),inet_ntoa(client2_addr.sin_addr));
      }
}


