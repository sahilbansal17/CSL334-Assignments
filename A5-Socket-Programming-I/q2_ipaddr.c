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
void populate_ipaddr(char *ip, struct sockaddr_in * serv_addr) {
	printf("The IPA when populating into the structure is: %s\n", ip);
	inet_aton(ip, &serv_addr->sin_addr);
}

// function to display the ip address populated in the serv_addr structure
void display_ipaddr(struct sockaddr_in serv_addr) {
	char *ip;
	ip = inet_ntoa(serv_addr.sin_addr);
	printf("The populated IPA is: %s\n", ip);
}

int main(int argc, char **argv) {

	struct sockaddr_in serv_addr;

	if (argc != 2) {
		perror("Usage: ipaddr <IP address>");
	}

	// zeroise the structure
    bzero(&serv_addr, sizeof(serv_addr));

    // populate the field in the serv_addr structure
	serv_addr.sin_family = AF_INET;
	// inet_aton(argv[1], &serv_addr.sin_addr);
	populate_ipaddr(argv[1], &serv_addr);

	// display the ip address already populated in serv_addr
	display_ipaddr(serv_addr);
	return 0;
}
