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

void encodeACK(char ack[4], short blockno){	
	short int tmp;
	tmp = htons(4);
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
	printf("send read req\n");

	char dp[516], data[512];
	short int opcode, blockno;
	int r = recvfrom(client_socket, dp, 516, 0, (struct sockaddr*)0, (int*)0);
	printf("r = %d\n",r);
	decodeDP(dp, r, &opcode, &blockno, data);
	printf("got dp block = %d\n", blockno);
	printf("========data==============\n%s\n===============\n", data);
	if(opcode==5){
		//err message
	}else if(opcode==3){
		int fd = open("myFile.txt", O_WRONLY);
		short int blockno2 = 1;
		short int blockno3;
		short int opcode2;
		if(fd<0){
			//create file
		}
		while(1){
			//printf("r = %d\n", r);
			write(fd, data, r-4);
			if(r<512){
				printf("\n\nDONE\n\n");
				break;
			}
			//memset(data, 0, sizeof(data));
			char ack[4];
			encodeACK(ack, blockno2);
			sendto(client_socket, ack, 4, 0, (struct sockaddr*)&server_address, sizeof(server_address));
			printf("send ack block %d\n", blockno2);
		
			r = recvfrom(client_socket, dp, 516, 0, (struct sockaddr*)0, (int*)0);
			printf("r = %d\n", r);
			decodeDP(dp, r, &opcode2, &blockno3, data);
			printf("got dp block = %d\n", blockno3);
			printf("========data==============\n%s\n===============\n", data);
			if(opcode2==3){
				blockno2++;
			}else if(opcode2==5){
				//error
			}
		}
	}


	close(client_socket);

	return 0;
}
