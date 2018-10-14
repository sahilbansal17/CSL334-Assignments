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

#define SERV_PORT 13 
#define MAXLINE 4096                 

void str_cli(FILE *fp, int sockfd) {
	char sendline[MAXLINE], recvline[MAXLINE];
    if (read(sockfd, recvline, MAXLINE) == 0) {
        perror("str_cli: server terminated prematurely");
    }
    fputs(recvline, stdout);
}

// function to display the ip address and port number of the server
int show_server_details(struct sockaddr_in serv_addr) {
	uint16_t port_no = ntohs(serv_addr.sin_port);
	// char *ip;
	// ip = inet_ntoa(serv_addr.sin_addr);

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
	struct sockaddr_in servaddr, cliaddr;
    
	if (argc != 2) {
		perror("usage: udpcli <IPaddress>");
		exit(0);
	}

	// change SOCK_STREAM to SOCK_DGRAM for using UDP instead of TCP
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
		perror("Socket creation error!");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	// servaddr.sin_addr.s_addr = INADDR_ANY;

	// connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	// there will be no connect call now, instead we will use recvfrom calls

	char *hello = "Hello from client"; 
	sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    printf("Hello message sent.\n"); 

	int recvd, serv_size;
	char *recv_data;
	if ((recvd = recvfrom(sockfd, recv_data, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &serv_size)) == -1) {
		perror("No data received from server");
		exit(0);
	}

	if (show_server_details(servaddr) == 1) {
		str_cli(stdin, sockfd);		/* do it all */
	};

	close(sockfd);
	exit(0);
}