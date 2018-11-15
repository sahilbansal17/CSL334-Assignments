Execution of the program (client to send ping request):

1. Server:
$ gcc -o server server.c
$ ./server &

2. Client
$ gcc -o client client.c
$ ./client server_IP_address server_port_number

Run $ ./client localhost 3993 because server is on localhost and port = 3993
