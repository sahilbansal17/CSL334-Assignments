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
#define MAXLINE 81                 

// this function takes as input, message from the client
// sends it to the server, prints the response
// exit when client types "Bye"
void str_messaging_system(FILE *fp, int sockfd) {
	char buff[MAXLINE]; 
    int n; 
    for ( ; ; ) { 
        bzero(buff, sizeof(buff)); 
        printf("Type your message : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') ; 

        // write to sockfd
        write(sockfd, buff, sizeof(buff)); 

        // clear the buffer and read from sockfd the response by the server
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 

        if ((strncmp(buff, "Exit", 4)) == 0) { 
            printf("You chose to quit...\n"); 
            break; 
        } 

        // print the daytime
        printf("Response from the server : %s\n", buff);
    } 
} 

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    // socket internet address structure
	struct sockaddr_in servaddr;
    
    // usage for the program
	if (argc != 2) {
		printf("usage: client <server_IP_address>\n");
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

	str_messaging_system(stdin, sockfd);		

	// close the socket descriptor
	close(sockfd);
    return 0;
}