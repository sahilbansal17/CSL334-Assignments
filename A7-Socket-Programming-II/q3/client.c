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

#define MAXLINE 4096                 

void str_hello_world(FILE *fp, int sockfd) {
	char buff[MAXLINE]; 
    bzero(buff, sizeof(buff)); 
    
	// read message from the server
    read(sockfd, buff, sizeof(buff)); 
    printf("Response from the server: %s\n", buff);
} 

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    // socket internet address structure
	struct sockaddr_in servaddr;
    
    // usage for the program
	if (argc != 3) {
		printf("usage: client <server_IPaddress server_port_number>\n");
		exit(EXIT_FAILURE);
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
	servaddr.sin_port = htons(atoi(argv[2]));
	// convert IP from ASCII to NBO and also populate in the servaddr.sin_addr field 
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	// connect to the server
	if ((connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
		perror("Connect error"); 
		exit(0);
	}

	str_hello_world(stdin, sockfd);		

	// close the socket descriptor
	close(sockfd);
    return 0;
}