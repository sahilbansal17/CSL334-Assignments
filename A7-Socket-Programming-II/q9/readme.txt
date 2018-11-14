Execution of the web server:

$ gcc -o server server.c
$ ./server &

In the web browser, open:
http://localhost:9908/path_to_the_file

OR, it can be tested using telnet:
$ telnet localhost 9908
Now, send a get request in the prompt:
e.g.: > GET /readme.txt HTTP/1.1