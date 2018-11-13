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

// socket internet address structure of the server
struct sockaddr_in servaddr;

// this function receives a response from the server and prints it to the screen
void str_hello_world(FILE *fp, int sockfd) {
	char buff[MAXLINE]; 
    bzero(buff, sizeof(buff)); 
    
	// read message from the server
	int n;
	socklen_t serv_size;
	if ((n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &serv_size)) == -1) {
		perror("No data received from server");
		exit(0);
	}
	
    printf("Response from the server: %s\n", buff);
} 

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    
    // usage for the program
	if (argc != 3) {
		printf("usage: client <server_IPaddress server_port_number>\n");
		exit(EXIT_FAILURE);
	}

	// the socket call to get the socket descriptor for the client
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
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
	
	// there will be no connect call now, instead we will use sendto and recvfrom call
	
	// send a hello message to the server
	char *hello = "Hello from client"; 
	if ((sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
		perror("Server not responding");
		exit(0);
	}; 
	printf("Hello message sent to the server.\n"); 
	
	str_hello_world(stdin, sockfd);		

	// close the socket descriptor
	close(sockfd);
    return 0;
}