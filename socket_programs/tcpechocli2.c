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
	if ((fgets(sendline, MAXLINE, fp)) != NULL)
		write(sockfd, sendline, strlen(sendline));
	if (read(sockfd, recvline, MAXLINE) == 0)
		perror("str_cli:server terminated prematurely");
	fputs(recvline, stdout);
}

int main(int argc, char **argv) {

	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2) 
		perror("Usage: tcpcli2 <IP address>");
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		perror("Socket creation error");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd);

	return 0;
}
