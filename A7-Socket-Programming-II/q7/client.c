// client socket program to implement ARP using the ioctl system call
// the program takes client's IPA in the network as the argument 
// and prints the hardware address of the client

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h> // for ifreq, arpreq structure passed as an argument to ioctl call
// #include <linux/sockios.h>

struct arpreq {
    struct sockaddr arp_pa;      /* protocol address */
    struct sockaddr arp_ha;      /* hardware address */
    int             arp_flags;   /* flags */
    struct sockaddr arp_netmask; /* netmask of protocol address */
    char            arp_dev[16];
};


int main(int argc, char **argv) {
    // socket descriptor
	int sockfd;
    // socket internet address structure
	struct sockaddr_in *servaddr;
    
    // interface request structure
    struct ifreq ifr; 
    
    // arp request structure
    struct arpreq apr;
    
    // usage for the program
	if (argc != 2) {
		printf("usage: client <host_IP_address>\n");
		exit(EXIT_FAILURE);
	}

	// the socket call to get the socket descriptor for the client
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
		perror("Socket creation error!");
		exit(0);
	}

	// zeroise the servaddr structure
	bzero(&servaddr, sizeof(servaddr));

    servaddr = malloc(sizeof(servaddr));

	// populate the fields of sockaddr
	servaddr->sin_family = AF_INET;
    
	// convert IP from ASCII to NBO and also populate in the servaddr.sin_addr field 
	inet_pton(AF_INET, argv[1], &servaddr->sin_addr);
    
    // set the IP address in the apr structure from the sockaddr_in servaddr structure
	apr.arp_pa = *(struct sockaddr *) servaddr;
    // printf("IPA: %s", inet_ntoa(servaddr->sin_addr));
    
    // int ioctl(int fd, unsigned long request, ...);
    // first argument: socket descriptor
    // second argument: device-dependent request code, for getting MAC address: SIOCGARP
    // third argument: generally a pointer to ifr structure or apr structure
    
    if (ioctl(sockfd, SIOCGARP, &apr) == -1) {
        perror("Error in ioctl call: ");
        exit(EXIT_FAILURE);
    }
    
    u_int8_t hw_addr[6]; // 6 integers to represent 6 bytes of MAC address
    // memcpy(hw_addr, ifr.ifr_hwaddr.sa_data, sizeof(hw_addr));
    memcpy(hw_addr, apr.arp_ha.sa_data, sizeof(hw_addr));
    printf("Hardware address: ");
    for (int i = 0; i < 6; i ++) {
        if (i != 5)
            printf("%02X:", hw_addr[i]);
        else
            printf("%02X\n", hw_addr[5]);
    }
    
	// close the socket descriptor
	close(sockfd);
    return 0;
}