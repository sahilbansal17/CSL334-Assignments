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

char ip_host[16];

// function to send http request to the server and print the response received
void http_handle(int sockfd, char *method, char *file_name) {

	char request[MAXLINE], response[MAXLINE]; 
    bzero(request, MAXLINE); 
    bzero(response, MAXLINE);
	
    // create a http request message
	strcat(request, method);
	strcat(request, " /");
	strcat(request, file_name);
	strcat(request, " HTTP/1.1\r\n\r\n");
    
	printf("The request message is: %s", request);
	
	// send the request message
	write(sockfd, request, sizeof(request));
	
	printf("The response message is:\n");
	// read the response message
	while (read(sockfd, response, sizeof(response)) != 0) {
		// print the response message
		printf("%s", response);
		bzero(response, MAXLINE);
	}
}

void host2ip(char *host_name) {
	struct addrinfo hints;
    struct addrinfo *result, *current_res;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // socket addresses for IPv4 address family
    hints.ai_socktype = 0; // socket addresses of any type can be returned
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    
    int status;
    // int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
    
    // node is the hostname
    // service is the port number
    // hints is structure of type addrinfo used to impose restrictions
    // res is a pointer to list of addrinfo structure, which contains the network addresses
    // that match the node and service given the hints
    status = getaddrinfo(host_name, NULL, &hints, &result);

    // status = 0 means that getaddrinfo succeeded
    
    if (status != 0) {
        perror("getaddrinfo call failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in *ip_addr;
    // for (current_res = result; current_res != NULL; current_res = current_res -> ai_next) {
    
    current_res = result;
    // get the value of sockaddr_in of the current result (addrinfo structure)
    ip_addr = (struct sockaddr_in *) current_res -> ai_addr;
    // convert from network byte order to presentation format
    inet_ntop(AF_INET, &ip_addr->sin_addr, ip_host, sizeof(ip_host));
    // printf("IP address: %s\n", ip_host);
}

int main(int argc, char **argv) {

	if (argc != 5) {
		printf("Usage: client host_name port_number method file_name\n");
		exit(EXIT_FAILURE);
	}

 	// socket descriptors
	int sockfd;

	// server sockaddr_in structures
	struct sockaddr_in servaddr;

	// the socket call
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket error");
		exit(0);
	}

 	// zeroize the servaddr structure
	bzero(&servaddr, sizeof(servaddr));

	// populate the values in the servaddr structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	
	// get IPA from the hostname
	
	host2ip(argv[1]);
	
	// convert IP from ASCII to NBO and also populate in the servaddr.sin_addr field 
	inet_pton(AF_INET, ip_host, &servaddr.sin_addr);
	// printf("The IP address of the host is: %s\n", ip_host);
	
	// connect to the server
	if ((connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
		perror("Connect error"); 
		exit(0);
	}
	
	http_handle(sockfd, argv[3], argv[4]);		
	
	// close the socket descriptor
	close(sockfd);
	
	return 0;
}
