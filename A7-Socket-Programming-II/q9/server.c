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
#define MAXLINE 4096
#define LISTENQ 10

// function to process the http request of the client
void http_handle(int sockfd) {

	char request[MAXLINE], response[MAXLINE]; 
    bzero(request, MAXLINE); 
    bzero(response, MAXLINE);
    
    // read the http request from the client
    read(sockfd, request, sizeof(request)); 
    
    // print the HTTP request 
    printf("HTTP Request:\n%s\n", request); 
   
    // process the HTTP request
    
    
    // send the http response to the client 
    write(sockfd, response, sizeof(response));
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

		// process the http request of the client
		http_handle(connfd);

		// close the connected socket 
		close(connfd); 
	}

	return 0;
}
