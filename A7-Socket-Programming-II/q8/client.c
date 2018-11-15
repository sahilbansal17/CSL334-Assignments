// FTP client, sends a request to our own FTP server 
// which responds with the contents of the file requested

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
#include <fcntl.h>

#define SERV_PORT 9908 
#define MAXLINE 1024                 

void ftp_get(FILE *fp, int sockfd) {
	char buff[MAXLINE]; 
    int n; 
    
    for ( ; ; ) {
        bzero(buff, sizeof(buff)); 
        n = 0; 

        printf("ftp_client > Type the path to the file to get or Exit to stop: "); 
        while ((buff[n++] = getchar()) != '\n') ;
        
        if ((strncmp(buff, "Exit", 4)) == 0) { 
            write(sockfd, buff, sizeof(buff));
            printf("You chose to quit...\n"); 
            break; 
        } 

        // write to sockfd
        write(sockfd, buff, sizeof(buff)); 

        // clear the buffer and read from sockfd the response by the server
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("ftp_client > Response from the ftp server:\n%s\n", buff);
    }
} 

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    // socket internet address structure
	struct sockaddr_in servaddr;
    
    // usage for the program
	if (argc != 2) {
		printf("usage: client <server IPaddress>\n");
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
	servaddr.sin_port = htons(SERV_PORT);
	// convert IP from ASCII to NBO and also populate in the servaddr.sin_addr field 
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	// connect to the server
	if ((connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
		perror("Connect error"); 
		exit(0);
	}

	ftp_get(stdin, sockfd);		

	// close the socket descriptor
	close(sockfd);
    return 0;
}