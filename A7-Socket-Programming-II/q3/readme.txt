Execution of the program:
1. Server
$ gcc -o server server.c
$ ./server &

2. Client
$ gcc -o client client.c
$ ./client localhost 3993

Please check the snapshots folder for the answers to the questions asked. 

1. To test the server even before writing the client program, we make a telnet connection with the server running on localhost on port 3993, as shown in telnet.png in Snapshots folder. 

2. If we run the client before running the server, we get the "Connect error: connection refused", as shown in client_before_server.png file.