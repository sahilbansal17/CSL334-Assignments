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

	char buffer[MAXLINE]; 
  	
	struct sockaddr_in cliaddr;

    for ( ; ; ) { 
        bzero(buffer, MAXLINE); 
  
        // read the message from client and copy it in buffer 
        int len, n; 
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
  	 	buffer[n] = '\0'; 

        // print buffer which contains the client contents 
        printf("From client: %s\n", buffer); 
       
        // if msg contains "0" then server exit and chat ended. 
        // if (strncmp("0", buffer, 1) == 0) { 
        // 	bzero(buffer, MAXLINE);
        // 	strcpy(buffer, "Exit");
        // 	sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
        //     printf("Connection closed by client...\n"); 
        //     break; 
        // } 

        bzero(buffer, MAXLINE);

        // get the day time
        time_t ticks;
		ticks = time(NULL);
		snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
	
        // send the buffer to client 
       	sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);     
    } 
}

int main(int argc, char **argv) {

	int sockfd;
	struct sockaddr_in servaddr, cliaddr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Socket error");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		perror("Bind error");
		exit(0);
	}

	// no listen call since it is a UDP server, only sendto and recvfrom calls

	// iterative server
	// so keep processing requests
	// automatically maintains a queue of requests from different clients

	// char buffer[MAXLINE]; 
    // char *hello = "Hello from server"; 

	// no accept call, only recvfrom call

	// receive hello message from the client
	// int len, n; 
	// n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
	// buffer[n] = '\0'; 

	// print the hello message from the client
	// printf("From client: %s\n", buffer); 

	// send hello message to the first client
	// sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
	// printf("Hello message sent.\n");  

	str_daytime(sockfd);
	
	close(sockfd); // parent closes connected socket

	return 0;
}
