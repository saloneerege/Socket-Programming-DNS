# Socket-Programming-DNS
Recursive DNS using TCP and UDP sockets
---------------------------------------------------------------------------
   The project involves the implementation of recursive DNS using TCP and UDP sockets. There are 3 servers and 2 clients wherein each client contacts Server1 in order to map the key to a particular value stored at the server 1. Every server has a set of the key-value mapping stored in it cache.
   The client 1 and 2 communicate with the server over UDP sockets . Server 1 uses the port number 21129 when communicating with the clients.
   If server 1 does not have the value for a particular key it will contact server 2. Server 1 uses a dynamic TCP port to communicate with Server 2 . Server 2 uses the port number 22129.
   In this case server 1 becomes the client and server 2 the next server. On receiving the value for a particular key from Server 2 , Server 1 will update its own cache and also provide the value to the client that asked for this value.
   If in case Server 2 does not contain the value for a particular key it will contact Server 3 to obtain the value. 
   In this case Server 2 uses a dynamic TCP port to communicate with the Server 3 which uses the port number 23129.
   Server 2 becomes the client in this case and Server 3 will return the value to Server 2. This value obtained is updated in Server 2 and Server 1s cache.
   The value is then returned to the client which asked for the key-value mapping.
   The client 1 and 2 will request the value for a particular key consecutively. Thus we do not need to handle concurrency.
   The clients will not know about this recursive process to obtain the key-value mapping.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   

i)   client1.c
     The user will provide the search word  for which the corresponding value is stored in the client1.txt file.
	 After accessing this file the client 1 will send the corresponding key to the server 1.
	 The client stores the search word to key mapping in a buffer which is allocated size dynamically. 
	 The client 1 first contacts the server 1 to obtain the value of a particular search word entered by the user.
	 
ii)  client2.c
     The user will provide the search word  for which the corresponding value is stored in the client2.txt file.
	 After accessing this file the client 2 will send the corresponding key to the server 1.
	 The client stores the search word to key mapping in a buffer which is allocated size dynamically.
	 The client 2 will contatc the server 1 only after the client 1 has finished obtaining the value for a particular search word.
	 
iii) dhtserver1.c
      The server 1 contains mapping of the key value pair in the server1.txt file. The server 1 stores these values from the txt file in a character array (buffer) .
	  The corresponding value is sent over the UDP socket connection created with the client. 
	  The server 1 differentiates between client 1 and 2 based on the value of num if (num%2 == 1) then it is client 1 and if ( num%2==0) then it is client 2. 
	  If the value is not available with Server 1 it will contact Server 2 wherein it uses a dynamic TCP port to communicate with the server 2. 
	  On receiving the corresponding value from the Server 2 Server 1 will update its own cache and add an entry for that key-value pair.
	  void paddr(unsigned char *a) function is used to print the IP address of server 1 in the dot notation.
	  
	 
iv)   dhtserver2.c 
	   The server 2 contains mapping of the key value pair in the server2.txt file. The server 2 stores these values from the txt file in a character array (buffer) .
	   The Server 2 replies to the Server 1 with the corresponding value available in its buffer over a TCP socket connection. If however this value is not available 
	   in its buffer it will contact Server 3. 
	   
v)	 dhtserver3.c 
	   The server 3 contains mapping of the key value pair in the server3.txt file. The server 3 stores these values from the txt file in a character array (buffer) .
	   The Server 3 replies to the Server 2 with the corresponding value available in its buffer over a TCP socket connection. 
	  
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   
 

     In order to run this program the makefile submitted can be used. The servers will run in the order : Server 1 , followed by server 2 and finally server 3.
	 The client 1 and 2 can run at the same time. However. Client 1 will first be executed to obtain the value of a particular search word .
	 After then client 2 can proceed to obtain the value for a particular search word.
	 The clients must be executed in an alternating fashion.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   	 
 The messages exchanged are GET and POST . 
     GET is used by the client in every case to send the key to the server.
     POST is sent with the value corresponding to the key sent by the server to the client.	
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   
	
    No idiosyncrasy observed in the execution of the project
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   	

    I have used code from Introduction to Socket Programming by Rutgers and Beej's tutorial
 	
	   
