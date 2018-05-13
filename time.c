#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
static void wakeup(int id){
	printf("WakeUp\n");
}
static void wakeup2(int id){
        printf("WakeUp2\n");
}

main(){
	int p;
	char buf[100];
	struct sigaction act;
	act.sa_handler=wakeup;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGALRM,&act,0);

	alarm(3);

	if((p=read(STDIN_FILENO,buf,100))<0){
                if (errno==EINTR){
                        printf("Interupt System Call\n");
                }else{
                        printf("General System Call Error\n");
                }
        }else{
                printf("System Call ok\n");
        }


	struct sigaction act2;
	act2.sa_handler=wakeup2;
        sigemptyset(&act2.sa_mask);
        act2.sa_flags=0;
        sigaction(SIGALRM,&act2,0);

	alarm(3);
	//Point of problem//
	//if((p=pause())<0){
	///*
	if((p=read(STDIN_FILENO,buf,100))<0){
		if (errno==EINTR){
			printf("Interupt System Call\n");
		}else{
			printf("General System Call Error\n");
		}
	}else{
		printf("System Call ok\n");
	}
	//*/

}

