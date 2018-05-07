#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(){

	int fd1 = open("a1.txt", O_RDONLY);

	if(fd1 < 0){
		return 1;
	}

	char buf[512];
	int count=0 ;
	int byteread = read(fd1, buf, sizeof(buf));
	if(byteread == 0){
		printf("!!! emply file !!!\n");
		return 1;
	}
	byteread = 1;

	while(byteread != 0){
	
		memset(buf, 0, sizeof(buf));
		byteread = read(fd1, buf, sizeof(buf));
		
		if(byteread == 0){
			printf("\n@@@ end @@@\n");
		}else{
			printf("buf = \n%s\n=====================\n", buf);
		}

	}

	return 0;
}
