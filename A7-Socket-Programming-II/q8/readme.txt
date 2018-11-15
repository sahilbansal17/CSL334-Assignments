Execution of the program (ftp client and ftp server):

1. FTP server
$ gcc -o server server.c
$ ./server &

2. FTP client
$ gcc -o client client.c
$ ./client localhost 