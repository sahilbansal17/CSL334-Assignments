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

union {
	short s;
	char c[sizeof(short)];
} un;

// function to check the byte ordering
void check_byte_ordering() {
	un.s = 0x0102;
	if (un.c[0] == '1' && un.c[1] == '2') {
		printf("This machine is big endian\n");
	}
	else {
		printf("This machine is little endian\n");
	}
}

int main(int argc, char **argv) {

	check_byte_ordering();

	return 0;
}
