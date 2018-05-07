#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <string.h>

int main(){

	char buf[512], rd[139], filename[128], mode[9];
	short int opcode, tmp;
	int len;

	opcode = 1;
	strcpy(filename, "read.txt");
	strcpy(mode, "octet");

	len = 2 + strlen(filename) + 1 + strlen(mode) + 1;

	tmp = htons(opcode);
	memcpy(rd, &tmp, 2);
	memcpy(rd+2, filename, strlen(filename)+1);
	memcpy(rd+2+strlen(filename)+1, mode, strlen(mode)+1);

	int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(client_socket == -1){
		printf("Fail to create socket\n");
		exit(1);
	}else{
		printf("Is create socket\n");
	}

	printf("client rd\n");
	printf("rd = %d:%s:%s\n", opcode, filename, mode);

	struct sockaddr_in server_address;
	bzero((char*)&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	sendto(client_socket, rd, len, 0, (struct sockaddr*)&server_address, sizeof(server_address));
	
	int r = recvfrom(client_socket, buf, 512, 0 ,(struct sockaddr*)0, (int*)0);

	printf("is recv %s\n", buf);

	close(client_socket);
	

	return 0;
}
