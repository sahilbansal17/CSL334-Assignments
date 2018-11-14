Execution of the program (client to send ping request):

$ gcc -o client client.c
$ ./client server_IP_address server_port_number

To test on local server:
$ gcc -o server server.c
$ ./server &
$ ./client localhost 3993