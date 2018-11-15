// FTP server, if file requested has size > 1024 bytes, it responds with 
// "File Size Greater Than 1 KB"
// and waits for another request from client
// otherwise sends the file 
// Connection is terminated when client does so

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
#include <fcntl.h>

#define SERVPORT 9908
#define MAXLINE 1024
#define LISTENQ 10

void ftp_handle(int sockfd) {

	char buff[MAXLINE], file_data[MAXLINE]; 
    bzero(buff, MAXLINE); 
	bzero(file_data, MAXLINE);

	for ( ; ; ) {
	    // read the path of requested file from client and copy it in buffer 
	    read(sockfd, buff, sizeof(buff)); 
		
		if ((strncmp(buff, "Exit", 4)) == 0) { 
            printf("Connection terminated by client...\n"); 
            break; 
        } 
		
	    // print buffer which contains the client contents 
	    printf("From client: %s\n", buff); 
	  
	  	// extract file name from buff using strtok
		char *file_name;
	  	file_name = strtok(buff, "\n");
		int fd = open(file_name, O_RDONLY);
		bzero(buff, MAXLINE);
		if (fd < 0) {
			strcpy(buff, "Invalid File\n");
			write(sockfd, buff, sizeof(buff));
			bzero(buff, MAXLINE);
			continue;
		}
		else {
			// find the size of the file
			// int size = 0;
			// fseek(fp, 0, 2);
			// size = ftell(fp);
			// fclose(fp);
			
			int n;
			
            if ( (n = read(fd, file_data, sizeof(file_data))) != 0) {
				// size > 1024 bytes
				bzero(buff, MAXLINE);
				strcpy(buff, "File Size Greater Than 1 KB\n");
				write(sockfd, buff, sizeof(buff));
				bzero(buff, MAXLINE);
				continue;
            }
			close(fd);
			
			// otherwise file size <= 1024 bytes
            write(sockfd, file_data, sizeof(file_data));
            bzero(file_data, sizeof(file_data));
		}
	}
}

int main(int argc, char **argv) {

 	// socket descriptors
	int listenfd, connfd;

	// length for the client structure
	socklen_t clilen;

	// client and server sockaddr_in structures
	struct sockaddr_in cliaddr, servaddr;

	// the socket call to get the listening socket descriptor
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket error");
		exit(0);
	}

 	// zeroize the servaddr structure
	bzero(&servaddr, sizeof(servaddr));

	// populate the values in the servaddr structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	// bind the listening socket descriptor to the servaddr structure
	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		perror("Bind error");
		exit(0);
	}

	// listen atmost to LISTENQ connections 
	if ((listen(listenfd, LISTENQ)) == -1) {
		perror("Listen error");
		exit(0);
	}

	// iterative server
	// so keep processing requests
	// automatically maintains a queue of requests from different clients
	for ( ; ; ) {
		clilen = sizeof(cliaddr);

		// accept the connection from client
		if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) == -1) {
			perror("Accept error");
			exit(0);
		}

		// process the ftp requests of the client
		ftp_handle(connfd);

		// close the connected socket 
		close(connfd); 
	}

	return 0;
}
