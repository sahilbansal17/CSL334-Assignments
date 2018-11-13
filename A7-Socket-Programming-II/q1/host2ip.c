#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERV_PORT 13 
#define MAXLINE 4096  

int main(int argc, char **argv) {

	if (argc < 2) {
        printf("Usage: ip <hostname>\n");
        exit(EXIT_FAILURE);
	}

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
    status = getaddrinfo(argv[1], NULL, &hints, &result);

    // status = 0 means that getaddrinfo succeeded
    
    if (status != 0) {
        perror("getaddrinfo call failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in *ip_addr;
    char ip_host[16];
    // for (current_res = result; current_res != NULL; current_res = current_res -> ai_next) {
    
    current_res = result;
    // get the value of sockaddr_in of the current result (addrinfo structure)
    ip_addr = (struct sockaddr_in *) current_res -> ai_addr;
    // convert from network byte order to presentation format
    inet_ntop(AF_INET, &ip_addr->sin_addr, ip_host, sizeof(ip_host));
    printf("IP address: %s\n", ip_host);
    // printf("Canonical name: %s\n", current_res->ai_canonname);
    
    // }
	return 0;
}