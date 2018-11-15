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

// function to process the http request of the client
void http_handle(int sockfd) {

	char request[MAXLINE], response[MAXLINE], file_data[MAXLINE], buffer[MAXLINE]; 
    bzero(request, MAXLINE); 
    bzero(response, MAXLINE);
    bzero(buffer, MAXLINE);
    bzero(file_data, MAXLINE);
    // read the http request from the client
    read(sockfd, request, sizeof(request)); 
    
    // process the HTTP request
    
    // STEP 1: Extract the first line of the request and then the 
    // method, url and version from this line
    char *first_line = strtok(request, "\n");
    // printf("first_line:%s\n", first_line);
    
    char *method = strtok(first_line, " "), *url, *version, *file_name;
    int i = 0, status_code;
    long length;
    while (first_line != NULL) {
        if (i == 0) {
            url = strtok(NULL, " ");
            first_line = url;
        }
        else if (i == 1) {
            version = strtok(NULL, "\r");
            first_line = version;
        }
        else {
            first_line = strtok(NULL, " ");
        }
        i ++;
    }
    printf("Method: %s, URL: %s, Version: %s\n", method, url, version);
    
    // STEP 2: Compare the method and the versions 
    if (strcmp(method, "GET") == 0 && strcmp(version, "HTTP/1.1") == 0) {
        // get the requested file
        file_name = strtok(url, "/");
    
        // STEP 3: Create an HTTP response containing the requested file
        // preceded by header lines
        
        strcat(response, version);
        strcat(response, " ");
        
        int fd = open(file_name, O_RDONLY);
        if (fd < 0) {
            status_code = 404;
            strcat(response, "404 Not Found\r\n");
            strcat(response, "Content-Type: text/plain\r\n\r\n");
            // send the http response to the client 
            strcat(response, "404 NOT FOUND\r\n");
            write(sockfd, response, sizeof(response)); 
        }
        else {
            status_code = 200;
            
            strcat(response, "200 OK\r\n");
            strcat(response, "Content-Type: text/plain\r\n\r\n");
            // send the http response to the client 
            write(sockfd, response, sizeof(response)); 
            
            /* read and send the content simultaneously */
            int n;
            while ( (n = read(fd, file_data, sizeof(file_data))) != 0) {
                write(sockfd, file_data, sizeof(file_data));
                bzero(file_data, sizeof(file_data));
            }
			close(fd);
        }
    }
    else {
        printf("Not a valid GET request!\n");
        strcpy(buffer, "Invalid request.");
        write(sockfd, buffer, sizeof(buffer));
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

		// process the http request of the client
		http_handle(connfd);

		// close the connected socket 
		close(connfd); 
	}

	return 0;
}
