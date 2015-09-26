all:
	gcc -o server1 dhtserver1.c -lsocket -lnsl -lresolv
	gcc -o server2 dhtserver2.c -lsocket -lnsl -lresolv
	gcc -o server3 dhtserver3.c -lsocket -lnsl -lresolv
	gcc -o client1 client1.c -lsocket -lnsl -lresolv
	gcc -o client2 client2.c -lsocket -lnsl -lresolv

clean:
	rm -f server1 server2 server3 client1 client2
