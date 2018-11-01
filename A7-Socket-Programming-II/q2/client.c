#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERV_PORT 9908 
#define MAXLINE 4096                 

// this function first sends a hello message to the client
// and prints the message received from the server 
// along with the time of the day
void str_daytime(FILE *fp, int sockfd) {
	char buff[MAXLINE]; 
    int n; 
    
    bzero(buff, sizeof(buff)); 
    printf("Sending hello message to the server: "); 
    n = 0; 
    // while ((buff[n++] = getchar()) != '\n') ;
    strcpy(buff, "Hello Server\n"); 

    // write to sockfd
    write(sockfd, buff, sizeof(buff)); 

    // clear the buffer and read from sockfd the response by the server
    bzero(buff, sizeof(buff)); 
    read(sockfd, buff, sizeof(buff)); 
    printf("Response from the server: %s\n", buff);
    
    // read the daytime response and print to the screen
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("Day time response from the server: %s\n", buff);
} 

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    // socket internet address structure
	struct sockaddr_in servaddr;
    
    // usage for the program
	if (argc != 2) {
		perror("usage: client <server IPaddress>");
		exit(0);
	}

	// the socket call to get the socket descriptor for the client
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation error!");
		exit(0);
	}

	// zeroise the servaddr structure
	bzero(&servaddr, sizeof(servaddr));

	// populate the fields of sockaddr
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	// convert IP from ASCII to NBO and also populate in the servaddr.sin_addr field 
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	// connect to the server
	if ((connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
		perror("Connect error"); 
		exit(0);
	}

	str_daytime(stdin, sockfd);		

	// close the socket descriptor
	close(sockfd);
    return 0;
}