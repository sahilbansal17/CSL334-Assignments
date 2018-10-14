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

#define SERV_PORT 13
#define MAXLINE 4096

// function to display the allocated IPA and the port number
void print_allocated_addr (int sockfd, struct sockaddr_in my_addr) {
    // prototype for the getsocket name function is:
    // int getsocketname(int sockfd, struct sockaddr * addr, socklen_t * addrlen)

	char ip[16];

    uint16_t port_no;

    //////////////////////////////////////////////////////
    printf("\nBefore getsockname call:\n");
    inet_ntop(AF_INET, &my_addr.sin_addr, ip, sizeof(ip));
    port_no = ntohs(my_addr.sin_port);

    printf("The allocated IPA is: %s\n", ip);
    printf("The allocated port no. is: %d\n", port_no);

    int sock_name;
    socklen_t my_addr_len = sizeof(my_addr);
    if ((sock_name = getsockname(sockfd, (struct sockaddr *) &my_addr, &my_addr_len)) == -1) {
        perror("Error in getsockname");
    } 

    /////////////////////////////////////////////////////
	printf("\nAfter getsockname call:\n");
    
    inet_ntop(AF_INET, &my_addr.sin_addr, ip, sizeof(ip));
    port_no = ntohs(my_addr.sin_port);

    printf("The allocated IPA is: %s\n", ip);
    printf("The allocated port no. is: %d\n", port_no);
}

int main(int argc, char **argv) {

	int sockfd;
	struct sockaddr_in cli_addr, serv_addr;

	if (argc != 2) {
		perror("Usage: getsockname <IP address of the server>");
	}

    // socket call
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		perror("Socket creation error");
	// zeroise the structures
    bzero(&cli_addr, sizeof(cli_addr));
    bzero(&serv_addr, sizeof(serv_addr));

    // populate the fields in the cli_addr structure
	cli_addr.sin_family = AF_INET;
	// choosing an unused port at random
	cli_addr.sin_port = htons(0);
	// tell kernel to automatically fill in the IPA of the machine 
	cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// populate the fields in the serv_addr structure
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

	// display the client address before the connect call is issued
	printf("Before the connect call:\n");
    print_allocated_addr (sockfd, cli_addr);

	// connect to the server
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		perror("Connect error");
	};

	printf("\nAfter the connect call:\n");
	// display the client address after the connect call is issued
	print_allocated_addr(sockfd, cli_addr);
	return 0;
}
