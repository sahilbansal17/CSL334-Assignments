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
	struct hostent *host;

	if (argc < 2) {
        printf("Usage: ip <hostname1 hostname2 hostname3 ...>\n");
        exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i ++) {
		// prototype:
		// struct hostent *gethostbyname (const char *hostname)
		host = gethostbyname(argv[i]);

		if (!host) {
	        printf("Lookup Failed: %s\n", hstrerror(h_errno));
	        return 0;
		}
		printf("host_name: %s\n", host->h_name);

		int j = 0;
		while(host->h_addr_list[j] != NULL) {
	        printf("host_addr_list[%d]: %s\n", j, inet_ntoa( (struct in_addr) * (struct in_addr *)host->h_addr_list[j] ) );
	        j++;
		}
	} 

	return 0;
}