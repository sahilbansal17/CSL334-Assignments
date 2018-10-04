#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVPORT 9339
#define MAXLINE 4096
#define LISTENQ 10

void str_echo(int sockfd) {

	ssize_t n;
	char line[MAXLINE];
	if ( (n = read(sockfd, line, MAXLINE)) == 0)
		return; // connection closed by the other end
	strcat(line, "Yes, Hello Client\n");
	write(sockfd, line, n + 18);
}

int main(int argc, char **argv) {

	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket error\n");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
		perror("Bind error\n");
	if ((listen(listenfd, LISTENQ)) == -1)
		perror("Listen error\n");

	for ( ; ; ) {

		clilen = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) == -1)
			perror("Accept error\n");
		if ((childpid = fork()) == 0) {
			// child process
			close(listenfd); // close the listening descriptor
			str_echo(connfd);
			exit(0);
		}
		close(connfd); // parent closes connected socket
	}

}
