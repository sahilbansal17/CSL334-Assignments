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

// this function sends a request to the server to give the daytime 
// and in response gets the daytime
// exit when server responds with "Exit"
void str_daytime(FILE *fp, int sockfd) {
	char buff[MAXLINE]; 
    int n; 
    for ( ; ; ) { 
        bzero(buff, sizeof(buff)); 
        printf("Enter any character except \"0\" to get the daytime : "); 
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
        printf("Day time response from the server : %s", buff);
    } 
} 

// function to display the ip address and port number of the server
int show_server_details(struct sockaddr_in serv_addr) {
	uint16_t port_no = ntohs(serv_addr.sin_port);

	struct hostent *host;
	host = gethostbyaddr(&serv_addr.sin_addr, sizeof(serv_addr.sin_addr), AF_INET);
	if (!host) {
		printf("Kindly enter IP address instead of host name\n");
		return 0;
	}
	printf("Port no: %d, Hostname: %s\n", port_no, host->h_name);
	return 1;
}

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    // socket internet address structure
	struct sockaddr_in servaddr;
    
	if (argc != 2) {
		perror("usage: tcpcli <IPaddress>");
		exit(0);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation error!");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr); 
	
	if ((connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
		perror("Connect error"); 
		exit(0);
	}

	if (show_server_details(servaddr) == 1) {
		str_daytime(stdin, sockfd);
	}

	close(sockfd);
    return 0;
}