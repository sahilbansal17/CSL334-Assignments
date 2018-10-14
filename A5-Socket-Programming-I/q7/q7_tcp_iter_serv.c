#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SERVPORT 9908
#define MAXLINE 4096
#define LISTENQ 10

// this function takes as input from client the request to display daytime
// and then responds with day time if it is a valid request
// otherwise exits when the client gives "0" as input 
void str_daytime(int sockfd) {

	char buff[MAXLINE]; 
  
    for ( ; ; ) { 
        bzero(buff, MAXLINE); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
       
        // if msg contains "0" then server exit and chat ended. 
        if (strncmp("0", buff, 1) == 0) { 
        	bzero(buff, MAXLINE);
        	strcpy(buff, "Exit");
        	write(sockfd, buff, sizeof(buff));
            printf("Connection closed by client...\n"); 
            break; 
        } 

        bzero(buff, MAXLINE);

        // get the day time
        time_t ticks;
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	
        // send the buffer to client 
        write(sockfd, buff, sizeof(buff));
    } 
}

int main(int argc, char **argv) {

	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket error");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		perror("Bind error");
		exit(0);
	}

	if ((listen(listenfd, LISTENQ)) == -1) {
		perror("Listen error");
		exit(0);
	}

	// iterative server
	// so keep processing requests
	// automatically maintains a queue of requests from different clients
	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) == -1) {
			perror("Accept error");
			exit(0);
		}
		str_daytime(connfd);
		close(connfd); // parent closes connected socket
	}

}
