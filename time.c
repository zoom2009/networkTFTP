#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
static void wakeup(int id){
	printf("WakeUp\n");
}

void timeW8(){
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
}

main(){
	timeW8();
	timeW8();
}

