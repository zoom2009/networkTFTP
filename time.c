#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
static void timeOut(int id){
	printf("\n!!!!!!!!!!!!wakeUp!!!!!!!!!!!!!!\n");
}
static void goNext(int id){
        printf("\n!!!!!!!!!!!!Gonext!!!!!!!!!!!!!!\n");
}



void timeW8(){
	int p;
        char buf[100];
        struct sigaction act;
        act.sa_handler=timeOut;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        sigaction(SIGALRM,&act,0);
}

void timeW82(){
        int p;
        char buf[100];
        struct sigaction act;
        act.sa_handler=goNext;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        sigaction(SIGALRM,&act,0);
}


main(){
	timeW8();
	alarm(5);
	timeW82();
	alarm(1);
	//alarm(0);
	printf("sss");
	//timeW8();
	while(1){
		//printf("1\n");
	}
	printf("done");
}
