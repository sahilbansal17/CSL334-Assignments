// simple stream server which sends "Hello, World" to the client
// UDP concurrent server

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

#define SERVPORT 3993
#define MAXLINE 4096

// this function first receives a hello message from the client
// and then responds with hello world message
void str_hello_world(int sockfd) {

	char buffer[MAXLINE]; 
  	
	struct sockaddr_in cliaddr;

    for ( ; ; ) { 
        bzero(buffer, MAXLINE); 
  
        // read the message from client and copy it in buffer 
        int n;
        socklen_t len; 
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
  	 	buffer[n] = '\0'; 
      
        bzero(buffer, MAXLINE);
        strcpy(buffer, "Hello, World\n");
        
        // send the buffer to client 
       	sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *) &cliaddr, len);     
    } 
}

int main(int argc, char **argv) {

 	// socket descriptor
	int sockfd;

	// client and server sockaddr_in structures
	struct sockaddr_in cliaddr, servaddr;

	// the socket call to get the listening socket descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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
	if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		perror("Bind error");
		exit(0);
	}

	// no listen call since it is a UDP server

	// no accept call in UDP server

	// simple stream server, sending hello world to the client on connectionless socket
	str_hello_world(sockfd);

	// close the socket descriptor
	close(sockfd); 

	return 0;
}
