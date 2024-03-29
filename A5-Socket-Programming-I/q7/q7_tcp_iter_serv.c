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

// this function takes as input from client the request to display daytime
// and then responds with day time if it is a valid request
// otherwise exits when the client gives "0" as input 
void str_daytime(int sockfd) {

	char buff[MAXLINE]; 
  
    for ( ; ; ) { 
        bzero(buff, MAXLINE); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
       
        // if msg contains "0" then server exit and chat ended. 
        if (strncmp("0", buff, 1) == 0) { 
        	bzero(buff, MAXLINE);
        	strcpy(buff, "Exit");
        	write(sockfd, buff, sizeof(buff));
            printf("Connection closed by client...\n"); 
            break; 
        } 

        bzero(buff, MAXLINE);

        // get the day time
        time_t ticks;
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	
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

		// process the daytime requests of the client
		str_daytime(connfd);

		// close the connected socket 
		close(connfd); 
	}

	return 0;
}
