#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

//TFTP Server

void decodeRW(int r, char code[139],short int *opcode, char filename[128], char mode[9]){

	short int tmp;
	char temp[128];
	memcpy(&tmp, code, 2);
	memcpy(temp, code+2, strlen(code+2)+1);
	memcpy(mode, code+2+strlen(temp)+1, r-strlen(temp)+3);
	memcpy(filename, temp, strlen(temp)+1);

	*opcode = ntohs(tmp);
}

void encodeDP(char dp[516], short int no, char data[512]){
	short int tmp = htons(3);
	short int tmno = htons(no);
	memcpy(dp, &tmp, 2);
	memcpy(dp+2, &tmno, 2);
	memcpy(dp+4, data, strlen(data)+1);
}

void decodeACK(char code[4], short int *opcode, short int *blockno){
	short int tmp;

	memcpy(&tmp, code, 2);
	*opcode = ntohs(tmp);
	memcpy(&tmp, code+2, 2);
	*blockno = ntohs(tmp);
}

int main(){

	int server_socket = server_socket = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in server_address, client_address;
	bzero((char*)&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	socklen_t addr_size = sizeof(client_address);
	char rd[139];
	int r = recvfrom(server_socket, rd, 139, 0, (struct sockaddr*)&client_address, &addr_size);

	char filename[128], mode[9];
	short int opcode;
	decodeRW(r, rd, &opcode, filename, mode);
	printf("got RW opcode = %d\n", opcode);

	if(opcode == 1){
		int fd = open(filename, O_RDONLY);
		if(fd<0){
			//file not found
		}
		char buf[512], dp[516];
		short int blockno = 1;
		int byteread;
		while((byteread = read(fd, buf, sizeof(buf)))!=0){
		
			encodeDP(dp, blockno, buf);
			sendto(server_socket, dp, byteread+4, 0, (struct sockaddr*)&client_address, addr_size);
			
			if(byteread<512){
				printf("\n\nDONE\n\n");
				break;
			}

			printf("send dp block %d\n", blockno);
			printf("=========data===========\n%s\n============\n", buf);
			memset(buf, 0, sizeof(buf));

			char ack[4];
			short int opcode2;
			short int blockno2;
			recvfrom(server_socket, ack, 4, 0, (struct sockaddr*)&client_address, &addr_size);
			decodeACK(ack, &opcode2, &blockno2);
			printf("got ack block %d\n", blockno2);
			if(opcode2==4){
				blockno++;
			}else if(opcode==5){
				//error
			}
			
		}
	
	}


	return 0;
}
