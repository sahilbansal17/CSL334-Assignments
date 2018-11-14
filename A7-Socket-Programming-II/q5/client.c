#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERV_PORT 9339
#define MAXLINE 4096

void str_cli (FILE *fp, int sockfd) {

	char sendline[MAXLINE], recvline[MAXLINE];
    
    for ( ; ; ) {
    	if ((fgets(sendline, MAXLINE, fp)) != NULL) {
    		write(sockfd, sendline, strlen(sendline));
        }
    	if (read(sockfd, recvline, MAXLINE) == 0) {
    		perror("str_cli:server terminated prematurely");
        }
    	fputs(recvline, stdout);
        bzero(sendline, MAXLINE);
        bzero(recvline, MAXLINE);
    }
}

int main(int argc, char **argv) {

    // socket descriptor
	int sockfd;
    // sockaddr internet structure for the server address
	struct sockaddr_in servaddr;

    
	if (argc != 2) {
		printf("Usage: client <server_IP_address>\n");
        exit(EXIT_FAILURE);
    }
    
    // socket call
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation error");
    }
    
    // zeroise the servaddr structure
	bzero(&servaddr, sizeof(servaddr));

    // set the values of the fields in the servaddr structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    // connect call
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        perror("Connect error");
    };
    
	str_cli(stdin, sockfd);

	return 0;
}
