Execution of the program:
1. Server
$ gcc -o server server.c
$ ./server &

2. Client
$ gcc -o client client.c
$ ./client localhost

Please check the snapshots folder for the answers to the questions asked.
Answers:

1. Starting the server before starting the client: "start_server.png" - 
The listening socket is in LISTEN state. The command used to find the state of the client and
server processes is "ps -ax | grep ./client" and "ps -ax | grep ./server".

2. Starting the client and when connection is established, before giving any inputs on the client: "start_client.png" -
There are two sockets for the server, parent process socket is in LISTEN state and child process
socket is in ESTABLISHED state. 
The socket for the client is in ESTABLISHED state.

The process of client (one process) is in S+ (suspended, foreground) state. 
The processes of server (two - child and parent) are in S (suspended, background) state.  

3. After sending input from the client side: "input_client.png" -
The socket and process states are same as in the previous case. 

4. Terminating the client: "terminate_client.png" -
The connected socket of server goes into TIME_WAIT state. 
There is only one process of server (parent) in S state. 
The process of client disappears.

After a few moments: "time_wait_over.png" - 
The TIME_WAIT state disappears.

5. A zombie process or defunct process is a process that has completed execution but still has an entry in the process table. The server child process is in zombie state after the client terminates the connection as it has disconnected with the client but is in TIME_WAIT state.