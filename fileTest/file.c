#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(){

	int fd1 = open("a1.txt", O_RDONLY);

	if(fd1 < 0){
		return 1;
	}

	char buf[512];
	int byteread, count=0 ;
	byteread = read(fd1, buf, sizeof(buf));
	printf("count : %d\n", count);
	printf("buf = \n%s\n\n", buf);
	while(byteread!=0){
		char buf[512];
		byteread = read(fd1,buf,sizeof(buf));
		count++;
		printf("count : %d\n", count);
        	printf("buf = \n%s\n\n", buf);

		if(byteread==0){
			printf("@@@ end @@@");
			break;
		}
	}

	return 0;
}
