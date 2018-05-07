#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <fcntl.h>
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

void decodeDP(char dp[516], int r, short int *opcode, short int *blockno, char data[512]){
	short int tmp;
	memcpy(&tmp, dp, 2);
	*opcode = ntohs(tmp);
	memcpy(&tmp, dp+2, 2);
	*blockno = ntohs(tmp);
	memcpy(data, dp+4, r-4);
}

void encodeACK(char ack[4], short int opcode, short blockno){	
	short int tmp;
	tmp = htons(opcode);
	memcpy(ack, &tmp, 2);
	tmp = htons(blockno);
	memcpy(ack+2, &tmp, 2);
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

	char buf[516];
	int r = recvfrom(client_socket, buf, 516, 0, (struct sockaddr*)0, (int*)0);
	short int opcode, blockno; 
	char data[512];
	
	decodeDP(buf, r, &opcode, &blockno, data);
	printf("recv : \n%d:%d:%s\n", opcode, blockno, data);
	
	if(opcode == 3){
		int fd = open("out.txt", O_WRONLY);
		int w = write(fd, data, r-4);
		//printf("w = %d\n", w);
		char ack[4];
		encodeACK(ack, 4, 1);
		sendto(client_socket, ack, 4, 0, (struct sockaddr*)&server_address, sizeof(server_address));

	}
	
	close(client_socket);

	return 0;
}
