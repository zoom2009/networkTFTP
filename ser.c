#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(){
	
	char rd[139], filename[128], mode[9];
	short int opcode, tmp;
	int len;

	int server_socket;
	server_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if(server_socket == -1){
		printf("Fail to create server socket\n");
		exit(1);
	}else{
		printf("Server socket is created\n");
	}

	struct sockaddr_in server_address, client_address;
	bzero((char*)&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	socklen_t addr_size = sizeof(client_address);
	int r = recvfrom(server_socket, rd, 139, 0, (struct sockaddr*)&client_address, &addr_size);
	
	memcpy(&tmp, rd, 2);
	opcode = ntohs(tmp);

	memcpy(filename, rd+2, strlen(rd+2)+1);
	char myFilename[128];
	strcpy(myFilename, filename);

	memcpy(mode, rd+2+strlen(filename)+1, r-strlen(filename)+3);

	//printf("\nAfter\n");
	//printf("opcode = %d\n", opcode);
	//printf("filename = %s\n", filename);
	//printf("myFilename = %s\n", myFilename);
	//printf("mode = %s\n", mode);

	if(opcode==1){ // read request
		int myFile = open("read.txt", O_RDONLY);
		if(myFile < 0){
			printf("File not found\n");
		}
		char buf[512];
		int byteread = read(myFile, buf, sizeof(buf));
		if(byteread == 0){
			printf("Emply file\n");
		}
		printf("is read : \n%s", buf);
		sendto(server_socket, buf, byteread, 0, (struct sockaddr*)&client_address, addr_size);
		
		byteread = 1;
		
		/*
		while(byteread != 0){
			
			memset(buf, 0, sizeof(buf));
			byteread = read(myFile, buf, sizeof(buf));

			if(byteread == 0){
				printf("end of file");
			}else{
				sendto(server_socket, buf, byteread, 0, (struct sockaddr*)&client_address, addr_size);

			}

		}
		*/


	}

	return 0;
}
