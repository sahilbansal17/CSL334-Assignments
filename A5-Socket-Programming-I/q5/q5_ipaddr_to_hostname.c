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
        printf("Usage: host <IPA1 IPA2 IPA3 ...>\n");
        exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i ++) {
		// prototype:
		// struct hostent *gethostbyaddr (const void *addr, size_t len, int family)
		struct in_addr ipa;
		inet_aton(argv[i], &ipa);
		host = gethostbyaddr(&ipa, sizeof(ipa), AF_INET);

		if (!host) {
	        printf("Lookup Failed: %s\n", hstrerror(h_errno));
	        return 0;
		}

		int j = 0;
		while(host->h_addr_list[j] != NULL) {
	        printf("host_addr_list[%d]: %s\n", j, inet_ntoa( (struct in_addr) * (struct in_addr *)host->h_addr_list[j] ) );
	        j++;
		}
		printf("host_name: %s\n\n", host->h_name);

	} 

	return 0;
}