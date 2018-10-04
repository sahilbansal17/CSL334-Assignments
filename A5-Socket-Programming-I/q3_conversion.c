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

// function to populate the serv_addr with the IPA take input from user
void populate_ipaddr(char *ip, struct sockaddr_in serv_addr) {
	inet_aton(ip, &serv_addr.sin_addr);
}

// function to display the ip address populated in the serv_addr structure
void display_ipaddr(struct sockaddr_in serv_addr) {
	char *ip;
	ip = inet_ntoa(serv_addr.sin_addr);
	printf("The populated IPA is:%s\n", ip);
}

// function to convert and display the IPA and port no. to Network Byte Order
void convert_to_NBO(struct sockaddr_in serv_addr) {


}

// function to convert and display the IPA and port no. to Host Byte Order
void convert_to_HBO(struct sockaddr_in serv_addr) {


}

int main(int argc, char **argv) {

	int sockfd;
	struct sockaddr_in serv_addr;

	if (argc != 2) {
		perror("Usage: convert <IP address>");
	}

	// zeroise the structure
    bzero(&serv_addr, sizeof(serv_addr));

    // populate the field in the serv_addr structure
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = SERV_PORT;
	populate_ipaddr(argv[1], serv_addr);

	// display the ip address already populated in serv_addr
	display_ipaddr(serv_addr);
	return 0;
}
