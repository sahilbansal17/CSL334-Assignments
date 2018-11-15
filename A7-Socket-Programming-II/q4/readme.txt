Execution of the program:
1. Server
$ gcc -o server server.c
$ ./server &

2. Client
$ gcc -o client client.c
$ ./client localhost 3993

Answer to the question whether it is a concurrent or iterative server: 
This is not an iterative server as in an iterative server, one client can send more than one request on the same connection but no notion of connection exists in an iterative server.

It is also not an concurrent server as there are no separate child and parent processes, so simultaneous requests from different clients cannot be processed.