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
#define BUFSIZE 400
int main()
{
  FILE *fp2;
  long lSize;
  char *buffer2;
  long i;
  int sa_len;
  int rqst;
  int s;
  int j=0;
  int sockTCP1,sockTCP2,sockTCP3;
  int nread;
  char token[100],get1[100];
  struct sockaddr_in server_addr,server_TCP1,server_TCP2;
  char *ret;
  int pos;
  char *host="nunki.usc.edu";
  struct hostent *hp;
  char key1[50]=" ",post[100]="POST ",get[100]="GET ";
  int server2_port=22129;
  char nextget[50]="GET ";
  char result[BUFSIZE];
  result[0]=0;
  socklen_t alen;
  char inputbuf[BUFSIZE],post1[10]="",next_value[50]="";
  fp2=fopen("server2.txt","r");
  if(fp2==NULL)
   {
	 perror("Error opening the file");
     return(-1);
   }
  fseek( fp2 , 0L , SEEK_END);
  lSize = ftell( fp2 );
  rewind( fp2 );
  buffer2 = calloc( 1, 1024 );
  if( !buffer2 ) fclose(fp2),fputs("memory alloc fails",stderr),exit(1);
  if( 1!=fread( buffer2 , lSize, 1 , fp2) )
   fclose(fp2),free(buffer2),fputs("entire read fails",stderr),exit(1);
 // printf("%s",buffer2);
  if ((sockTCP3 = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
  {
	perror("Socket could not be created !!");
	return 0;
  }
  memset((char *)&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(server2_port);
  hp = gethostbyname(host);
  if (!hp)     //Reused from Rutgers
  {
	printf("\ncould not obtain address of %s\n", host);
	return 0;    
  }
   memcpy((void *)&server_addr.sin_addr,hp->h_addr_list[0],hp->h_length);
   printf("\n The Server 2 has TCP port number %d and IP address %s",ntohs(server_addr.sin_port),inet_ntoa( server_addr.sin_addr));
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
   //printf("\n Listening!!");
   alen = sizeof(server_TCP2);
   memset((char *)&server_TCP2,0,sizeof(server_TCP2));
   for(;;)
   {
     while ((rqst = accept(sockTCP3,(struct sockaddr *)&server_TCP2, &alen)) < 0)
	 {
		  if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) 
		                {
                                perror("accept failed");
                                exit(1);
                        } 
	 }		 
	 nread = read(rqst,inputbuf,BUFSIZE);
     printf("\n%s",inputbuf); 
	 sscanf(inputbuf,"%s %s",get1,token);
	 printf("\n The Server 2 has received a request with %s from the Server 1 with port number %d and IP address %s ",token,ntohs(server_TCP2.sin_port),inet_ntoa(server_TCP2.sin_addr));
     if(nread > 0)  
		 {
          //printf("\n%s",token);
			ret = strstr(buffer2,token); // strstr finds the substring in a string
			if(ret!=NULL)
			{
			  pos = ret -  buffer2;
              //printf("\n%d",pos);
			  while(buffer2[pos]!=' ')
				{
					pos++;
				}
			pos=pos+1;
			j=0;
			for(j=0;j<7;j++)
			{
			  key1[j]=buffer2[pos];
			  pos++;
			}
   /*while(buffer2[pos]!='\n')
   {
      key1[j]=buffer2[pos];
      j++;
      pos++;
   }
   */
			//printf("\n%s",key1); 
			strcat(post,key1);
			//printf("\n%s",post);
			printf("\n The Server 2 sends the reply %s to the Server 1 with port number %d and IP address %s ",post,ntohs(server_TCP2.sin_port),inet_ntoa(server_TCP2.sin_addr));
			write(rqst,post,BUFSIZE);    
			strcpy(post,"POST ");    // reinitialize post to only POST 
	}
	   
    else
	{
       if ((sockTCP1 = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	   {
		perror("Socket could not be created !!");
		return 0;
	   }
      memset((char *)&server_TCP1, 0, sizeof(server_TCP1)); // Reused from Beej's tutorial
	  server_TCP1.sin_family = AF_INET;
	  server_TCP1.sin_addr.s_addr = htonl(INADDR_ANY);
	  server_TCP1.sin_port = htons(0);
	  sa_len = sizeof(server_TCP1);
	
      if (bind(sockTCP1, (struct sockaddr *)&server_TCP1, sizeof(server_TCP1)) < 0) 
	  {
		perror("bind failed");
		return 0;
	  }
      // else printf("\nDONE !!!!");
      memset((char*)&server_TCP2, 0, sizeof(server_TCP2));
      server_TCP2.sin_family = AF_INET;
      server_TCP2.sin_port = htons(23129);
	   hp = gethostbyname(host);
      if (!hp) 
	  {
		printf("\ncould not obtain address of %s\n", host);
		return 0;    
	  }
	  memcpy((void *)&server_TCP2.sin_addr,hp->h_addr_list[0],hp->h_length);
	  memcpy((void *)&server_TCP1.sin_addr,hp->h_addr_list[0],hp->h_length);
      if (connect(sockTCP1, (struct sockaddr *)&server_TCP2, sizeof(server_TCP2)) < 0) 
	  {
    	perror("\nconnect failed");
	    return 0;
      }
   // strcat(nextget,get);
     strcat(nextget,token);  
	   if(getsockname(sockTCP1,(struct sockaddr *) &server_TCP1, &sa_len) == -1) 
	 {
		perror("getsockname() failed");
        return -1;
     }  
	 
     printf("\n The Server 2 sends the request %s to Server 3. \n The TCP port number is %d and the IP address is %s",nextget,ntohs(server_TCP1.sin_port),inet_ntoa(server_TCP1.sin_addr));	
     write(sockTCP1,nextget,BUFSIZE);
     strcpy(nextget,"GET ");
   // printf("\n NEXTGET DONE \n");
	//printf("\nBuffer before read::");
	//printf("\n%s",buffer2);
	//printf("\n");
     nread=read(sockTCP1,result,BUFSIZE-1);
     result[nread]=0;
	 //printf("\nBuffer after read::");
     //printf("\n%s",buffer2);
	 //printf("\n");
	// printf("%s",result);
	//printf("%d",nread);
	//printf("\nBefore Buffer!");
    //printf("%s",buffer2);
    //printf("\nAfter Buffer!");
    if(nread > 0)
    {
       sscanf(result,"%s %s",post1,next_value);
	   printf("\n The Server 2 received the %s from the Server 3 with port number %d and IP address %s \n The Server 2 closed the TCP connection with the Server 3",next_value,ntohs(server_TCP1.sin_port),inet_ntoa(server_TCP1.sin_addr));
	   strcat(buffer2,"\n");
	   strcat(buffer2,token);
	   strcat(buffer2," ");
	   strcat(buffer2,next_value);
       //printf("\n%s",buffer2);
	    write(rqst,result,BUFSIZE);  
       printf("\n The Server 2 sent the reply %s to the Server 1 with port number %d and IP address is %s \n",result,ntohs(server_TCP2.sin_port),inet_ntoa(server_TCP2.sin_addr));
    }
	close(sockTCP1);
   }
  }
} 
 
 

}
       


