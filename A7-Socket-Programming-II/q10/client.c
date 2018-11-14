// client ping program which sends 10 ping messages, determines and prints the RTT
// it waits upto 1 second for reply of a ping message otherwise assumes that the packet is lost
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 4096                 

// socket internet address structure of the server
struct sockaddr_in servaddr;

// this function sends 10 ping messages to the server
void str_ping(FILE *fp, int sockfd) {
	char buff[MAXLINE]; 
    bzero(buff, sizeof(buff)); 
    
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("Unable to set timeout of 1 seconds:");
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("Unable to set timeout of 1 seconds:");
    }
    
    clock_t t;
    int n;
    socklen_t serv_size;
    double time_taken;
    for (int i = 0; i < 10; i ++) {
        // send a hello message to the server
    	char *hello = "Hello from client"; 
        
         
    	if ((sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr))) == -1) {
    		perror("Send message: Timeout");
    	}; 
        t = clock();
    	printf("Message %d: Hello message sent to the server.\n", i + 1); 
        
    	// read message from the server
    	if ((n = recvfrom(sockfd, (char *)buff, MAXLINE, 0, (struct sockaddr *) &servaddr, &serv_size)) == -1) {
    		perror("Receive message: Timeout");
    	}
        else {
            t = clock() - t;
            printf("Message %d: Response received from the server.\n", i + 1);
            time_taken = 1000*((double)t)/CLOCKS_PER_SEC;
            printf("Round Trip Time is: %f ms\n\n", time_taken);
        }
    }

} 

int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    
    // usage for the program
	if (argc != 3) {
		printf("usage: client <server_IPaddress server_port_number>\n");
		exit(EXIT_FAILURE);
	}

	// the socket call to get the socket descriptor for the client
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
		perror("Socket creation error!");
		exit(0);
	}

	// zeroise the servaddr structure
	bzero(&servaddr, sizeof(servaddr));

	// populate the fields of sockaddr
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	// convert IP from ASCII to NBO and also populate in the servaddr.sin_addr field 
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	// there will be no connect call now, instead we will use sendto and recvfrom call
	
	str_ping(stdin, sockfd);		

	// close the socket descriptor
	close(sockfd);
    return 0;
}