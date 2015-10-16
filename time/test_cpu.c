/*
1 
*/
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
int main(int argc, char *argv[])
{
	//struct timespec real_tp, real_tp2;
	struct timeval tv;
	//clock_gettime(CLOCK_REALTIME, &real_tp);	
	//clock_gettime(CLOCK_REALTIME, &real_tp2);
	gettimeofday(&tv, NULL);
        printf("gettimdofday = {%ld, %ld}\n", tv.tv_sec, tv.tv_usec);
	int i = 0;
	for(i = 0; i < 100000; i++)
		gettimeofday(&tv, NULL);
	//gettimeofday(&tv2, NULL);
	//printf("CLOCK_REALTIME = {%ld, %ld}\n", real_tp.tv_sec, real_tp.tv_nsec);
	//printf("CLOCK_REALTIME = {%ld, %ld}\n", real_tp2.tv_sec, real_tp2.tv_nsec);
	printf("gettimdofday = {%ld, %ld}\n", tv.tv_sec, tv.tv_usec);
	//printf("gettimdofday = {%ld, %ld}\n", tv2.tv_sec, tv2.tv_usec);
}
