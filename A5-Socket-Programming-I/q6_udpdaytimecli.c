/* This client daytime program is supposed to be the first to be demonstrated amongst all the daytime client servers. 
This uses the default port no 13 on the server for the daytime service. The client then attempts to connect to port no 13
on the server, gets the daytime and prints it on the screen and terminates. Note that you may get permission errors with 
this code */

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
void show_server_details(struct sockaddr_in serv_addr) {
	uint16_t port_no = ntohs(serv_addr.sin_port);
	char *ip;
	ip = inet_ntoa(serv_addr.sin_addr);
	printf("Port no: %d, IPA: %s\n", port_no, ip);
}

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    // socket internet address structure
	struct sockaddr_in servaddr, cliaddr;
    
	if (argc != 2)
		perror("usage: daytimecli1 <IPaddress>");

	// change SOCK_STREAM to SOCK_DGRAM for using UDP instead of TCP
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	// connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	// there will be no connect call now, instead we will use recvfrom calls

	// bind the socket with the server address
	if ( bind(sockfd, (struct sockaddr *) & servaddr, sizeof(servaddr)) == -1)
		perror("Bind error:");

	ssize_t recvd;
	socklen_t cli_size; 
	char *recv_data;
	if ((recvd = recvfrom(sockfd, recv_data, MAXLINE, 0, (struct sockaddr *) &cliaddr, &cli_size)) == -1)
		perror("No data received from server: ");

	show_server_details(servaddr);
	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
    return 0;
}