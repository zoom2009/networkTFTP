#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <string.h>

//TFTP Client

int encodeRW(char bd[139], short int opcode, char filename[128], char mode[9]){

	short int tmp = htons(opcode);
	memcpy(bd, &tmp, 2);
	memcpy(bd+2, filename, strlen(filename)+1);
	memcpy(bd+2+strlen(filename)+1, mode, strlen(mode)+1);

	printf("bd = %d:%s:%s\n", opcode, filename, mode);

	return 2+strlen(filename)+1+strlen(mode)+1;
}

int main(){

	int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in server_address;
	bzero((char*)&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	char bd[139];
        int len = encodeRW(bd, 1, "read.txt", "octet");

	sendto(client_socket, bd, len, 0, (struct sockaddr*)&server_address, sizeof(server_address));

	close(client_socket);

	return 0;
}
