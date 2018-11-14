// Simple Instant Messaging System server code
// TCP iterative server
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SERVPORT 9908
#define MAXLINE 81
#define LISTENQ 10

// this function waits from messsage from the client 
// then responds with its own message (input from server)
// otherwise exits when the client gives "Bye" as input (Good bye message)
void str_messaging_system(int sockfd) {

	char buff[MAXLINE]; 
    int n;
    for ( ; ; ) { 
        bzero(buff, MAXLINE); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\n", buff);
        
        // if msg contains "Bye" then server exit and chat ended. 
        if (strncmp("Bye", buff, 3) == 0) { 
        	bzero(buff, MAXLINE);
        	strcpy(buff, "Exit");
        	write(sockfd, buff, sizeof(buff));
            printf("Connection closed by client...\n"); 
            break; 
        } 
        
        printf("Type your message: ");
        bzero(buff, MAXLINE); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') ; 

        // send the buffer to client 
        write(sockfd, buff, sizeof(buff));
    } 
}

int main(int argc, char **argv) {

 	// socket descriptors
	int listenfd, connfd;

	// length for the client structure
	socklen_t clilen;

	// client and server sockaddr_in structures
	struct sockaddr_in cliaddr, servaddr;

	// the socket call to get the listening socket descriptor
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket error");
		exit(0);
	}

 	// zeroize the servaddr structure
	bzero(&servaddr, sizeof(servaddr));

	// populate the values in the servaddr structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	// bind the listening socket descriptor to the servaddr structure
	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		perror("Bind error");
		exit(0);
	}

	// listen atmost to LISTENQ connections 
	if ((listen(listenfd, LISTENQ)) == -1) {
		perror("Listen error");
		exit(0);
	}

	// iterative server
	// so keep processing requests
	// automatically maintains a queue of requests from different clients
	for ( ; ; ) {
		clilen = sizeof(cliaddr);

		// accept the connection from client
		if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) == -1) {
			perror("Accept error");
			exit(0);
		}

		// process the messages of the client
		str_messaging_system(connfd);

		// close the connected socket 
		close(connfd); 
	}

	return 0;
}
