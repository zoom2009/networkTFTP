#include <stdio.h>
#include <time.h>
#define TIMEWAIT 5


int getCurrentTimeSec(){
	time_t now;
	struct tm *tm;
	now = time(0);
	tm = localtime(&now);

	return tm->tm_sec;
}

int main (int argc, char *argv[]) {
	
	int beginTime = getCurrentTimeSec();
	printf("Time Start at %d\n", beginTime);
    	printf("Time wait = %d\n", TIMEWAIT);

	for(;;){
		int currentTimeSec = getCurrentTimeSec();
		printf("now is : %d\n", currentTimeSec);
		if(currentTimeSec==beginTime+TIMEWAIT){
			printf("!!! Alarm !!!\n");
			break;
		}
	}

	return 0;
}
