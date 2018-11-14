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
    
    for ( ; ; ) {
    	if ( (n = read(sockfd, line, MAXLINE)) == 0)
    		return; // connection closed by the other end
    	strcat(line, "Yes, Hello Client\n");
    	write(sockfd, line, n + 18);
        bzero(line, MAXLINE);
    }
}

int main(int argc, char **argv) {
    
    // socket descriptors
	int listenfd, connfd;
    // child process id
	pid_t childpid;
    // length of the client socket structure
	socklen_t clilen;
    // sockaddr structures
	struct sockaddr_in cliaddr, servaddr;

    // socket call
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket error\n");
		exit(1);
	}

    // zeroise the servaddr structure
	bzero(&servaddr, sizeof(servaddr));
    // assigne the values to the fields of the servaddr structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

    // bind the listening socket descriptor to the servaddr structure
	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		perror("Bind error\n");
    }
    
    // listen 
	if ((listen(listenfd, LISTENQ)) == -1) {
		perror("Listen error\n");
    }

	for ( ; ; ) {
        // initialize the length of client socket structure
		clilen = sizeof(cliaddr);
        
        // accept call, returns the accepted descriptor, connfd
		if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) == -1) {
			perror("Accept error\n");
        }
        
		if ((childpid = fork()) == 0) {
			// child process
			close(listenfd); // close the listening descriptor
			str_echo(connfd);
			exit(0);
		}
		close(connfd); // parent closes connected socket
	}

    return 0;
}
