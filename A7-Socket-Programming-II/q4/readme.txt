Execution of the program:
1. Server
$ gcc -o server server.c
$ ./server &

2. Client
$ gcc -o client client.c
$ ./client localhost 3993

Answer to the question whether it is a concurrent or iterative server: 
This is an iterative server as in an iterative server, the server first responds the request of one client, then sends response to the requests of the next client and keeps on processing clients in this way.

It is not an concurrent server as there are no separate child and parent processes, so simultaneous requests from different clients cannot be processed.