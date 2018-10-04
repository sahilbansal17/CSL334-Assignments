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

void str_cli (FILE *fp, int sockfd) {

	char sendline[MAXLINE], recvline[MAXLINE];
	if ((fgets(sendline, MAXLINE, fp)) != NULL)
		write(sockfd, sendline, strlen(sendline));
	if (read(sockfd, recvline, MAXLINE) == 0)
		perror("str_cli:server terminated prematurely");
	fputs(recvline, stdout);
}

void print_allocated_addr (int sockfd, struct sockaddr* my_addr) {
    // prototype for the getsocket name function is:
    // int getsocketname(int sockfd, struct sockaddr * addr, socklen_t * addrlen)
    int sock_name;
    socklen_t my_addr_len = sizeof(my_addr);
    if ((sock_name = getsocketname(sockfd, my_addr, my_addr_len)) == -1) {
        perror("Error in getsocketname");
    } 
    printf("%d", sock_name);
}

int main(int argc, char **argv) {

	int sockfd;
	struct sockaddr_in cli_addr;

    // socket call
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		perror("Socket creation error");
	// zeroise the structure
    bzero(&cli_addr, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	// choosing an unused port at random
	cli_addr.sin_port = htons(0);
	// tell kernel to automatically fill in the IPA of the machine 
	// inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // function to display the allocated IPA and the port number
	print_allocated_addr (sockfd, (struct sockaddr *) &cli_addr);
	return 0;
}
