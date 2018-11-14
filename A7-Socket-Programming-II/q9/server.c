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
#define MAXLINE 4096
#define LISTENQ 10

// funtion to send file 
int sendFile(FILE* fp, char* buf, int s) 
{ 
    int i, len; 
    if (fp == NULL) { 
        strcpy(buf, "\0"); 
        return 1; 
    } 
  
    char ch; 
    for (i = 0; i < s; i++) { 
        ch = fgetc(fp);  
        buf[i] = ch; 
        if (ch == EOF) 
            return 1; 
    } 
    return 0; 
} 

// function to process the http request of the client
void http_handle(int sockfd) {

	char request[MAXLINE], response[MAXLINE], file_data[MAXLINE], buffer[MAXLINE]; 
    bzero(request, MAXLINE); 
    bzero(response, MAXLINE);
    bzero(file_data, MAXLINE);
    bzero(buffer, MAXLINE);
    // read the http request from the client
    read(sockfd, request, sizeof(request)); 
    
    // print the HTTP request 
    // printf("HTTP Request:\n%s\n", request); 
   
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
        // printf("\nFILE NAME: %s\n", file_name);
        
        FILE *fp = fopen(file_name, "r");
        if (fp == NULL) {
            status_code = 404;
        }
        else {
            status_code = 200;
            
            // copy file into file_data
            int s = sendFile(fp, file_data, MAXLINE);
            
            /* Sending file using fgets */
            // while (fgets(buffer, sizeof(buffer), fp)) {
            //     strcat(file_data, buffer);
            //     strcat(file_data, "\n");
            // }
            // strcat(file_data, "\0");
            
            /* Sending file using read function */
            // off_t file_length = lseek(fd, 0, SEEK_END);
            // lseek(fd, 0, SEEK_SET);
            // file_data = malloc(file_length);
            // if (file_data) {
            //     read(fd, file_data, sizeof(file_data));
            // }
            // else {
            //     printf("\nFile read error\n");
            // }
            
            /* Sending file using fread function */
            // fseek(fp, 0, SEEK_END);
            // length = ftell(fp);
            // fseek(fp, 0, SEEK_SET);
            // file_data = malloc(length + 1);
            // if (file_data) {
            //     fread(file_data, 1, length, fp);
            //     printf("\n FILE READ \n");
            //     file_data[length] = '\0';
            // }
            // else {
            //     printf("\nFile Read error.\n");
            // }
            // fclose(fp);
        }
        
        // STEP 3: Create an HTTP response containing the requested file
        // preceded by header lines
        
        strcat(response, version);
        strcat(response, " ");
        if (status_code == 200) {
            strcat(response, "200 OK\r\n");
        }
        else if (status_code == 404){
            strcat(response, "404 Not Found\r\n");
        }
        strcat(response, "Content-Type: text/plain\r\n\r\n");
        strcat(response, file_data);
        // send the http response to the client 
        write(sockfd, response, sizeof(response)); 
    }
    else {
        printf("Not a valid GET request!\n");
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
