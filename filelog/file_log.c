/*
 * file_log.c
 *
 *  Created on: Jul 8, 2015
 *      Author: zhoulp
 */
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "file_log.h"

FILE *__log_out_file = NULL;
int __log_out_priority = 0;
static int write_count = 0;
static int flag_fflush = 0;
static unsigned int file_max_sizes = 0;
//static unsigned int file_max_lines = 0;
static pthread_mutex_t log_mtx = PTHREAD_MUTEX_INITIALIZER;
static unsigned int threshold_switch = 10000;
static unsigned int threshold_switch_size = 1024*1024;//default 1MB
static unsigned int total_num = 0;
char file_name[16][64];
//static unsigned int cur_file_size = 0;
static char prefix[64] = { 0 };


char LEVEL[6][16] = {
		{"LOG_FATAL"},
		{"LOG_ERROR"},
		{"LOG_WARN "},
		{"LOG_INFO "},
		{"LOG_DEBUG"},
		{""}};

static void Log_switch()
{
	printf("Log_switch \n");
	struct stat stat_info;

	sprintf(file_name[0], "%s.%d", prefix, 0);
	stat(file_name[0], &stat_info);
	printf("%s: %ld*%ld=%ld, %ld\n", file_name[0],
			stat_info.st_blksize, stat_info.st_blocks, stat_info.st_blksize * stat_info.st_blocks, stat_info.st_size);
	if(stat_info.st_size > threshold_switch_size)
	{
		if(__log_out_file)
		{
			fclose(__log_out_file);
			__log_out_file = NULL;
		}

		int i = total_num-1;
		for(;i  >= 1; i--){
			if(i == total_num -1){
				unlink(file_name[i]);
			}
			printf("file_name[%d] = %s , file_name[%d] = %s\n", i-1, file_name[i-1], i, file_name[i]);
			rename(file_name[i -1], file_name[i]);
		}
		__log_out_file = fopen(file_name[0], "w+");

	}

}
static void Log_switch_bak()
{
	printf("Log_switch_bak \n");


	int i = total_num-1;
	for(;i  >= 1; i--){
		if(i == total_num -1){
			unlink(file_name[i]);
		}
		printf("file_name[%d] = %s , file_name[%d] = %s\n", i-1, file_name[i-1], i, file_name[i]);
		rename(file_name[i -1], file_name[i]);
	}


}
int Log_init(const char *prefix_str, unsigned int log_file_num, unsigned int max_size, int priority, int buffer_flag)
{
	if(prefix == NULL || log_file_num < 1 || log_file_num > 16)
	{
		return -1;
	}
	if(buffer_flag){
		flag_fflush = 0;
	}else
	{
		flag_fflush = 1;
	}
	if(max_size < threshold_switch_size)
		file_max_sizes = threshold_switch_size;
		//file_max_lines = max_lines;
	else {
		file_max_sizes = max_size;
		threshold_switch_size = max_size;
		//file_max_lines = max_lines;
	}

	pthread_mutex_lock(&log_mtx);
	memset(file_name, 0x00, sizeof(file_name));

	write_count = 0;
	total_num = log_file_num;
	__log_out_priority = priority;
	strcpy(prefix, prefix_str);
	int i = total_num-1;
	for(;i  >= 0; i--){
		sprintf(file_name[i], "%s.%d", prefix, i);
		printf("file_name[%d] = %s\n", i, file_name[i]);
	}
	Log_switch_bak();
	__log_out_file = fopen(file_name[0], "w+");
	pthread_mutex_unlock(&log_mtx);
	return 0;
}

void Log_close()
{
	__log_out_priority = 0;
	if(__log_out_file)
	{
		fclose(__log_out_file);
		__log_out_file = NULL;
	}

}


inline void Log_write(int priority, const char* a_format,...)
{
	//if(priority > __log_out_priority || __log_out_file == NULL) return;
	va_list va;
	va_start(va, a_format);
	//printf("Log_write1 \n");
	pthread_mutex_lock(&log_mtx);
	write_count++;
	if(write_count % threshold_switch == 0)
	{
		write_count = 0;
		Log_switch();
	}
	//printf("Log_write2 \n");

	vfprintf(__log_out_file, a_format, va);
	if(flag_fflush)fflush(__log_out_file);
	pthread_mutex_unlock(&log_mtx);
	va_end(va);
}
void *thread_write1(void *param)
{
	int count = 0;
	do
	{
		LOG_ALL(LOG_INFO, "thread_write1_%d\n", count);
		count++;
	}while(count < 500000);

	return NULL;
}
void *thread_write2(void *param)
{
	int count = 0;
	do
	{
		LOG_ALL(LOG_WARN, "thread_write2_%d\n", count);
		count++;
	}while(count < 500000);

	return NULL;
}
void *thread_write3(void *param)
{
	int count = 0;
	do
	{
		LOG_ALL(LOG_ERROR, "thread_write2_%d\n", count);
		count++;
	}while(count < 500000);

	return NULL;
}
int main1()
{
	Log_init("log_test", 3, 1024*1024*10, LOG_INFO, LOG_WITH_BUFFER);
	pthread_t pid1, pid2;
	struct timeval tv, tv_bak;

	gettimeofday(&tv_bak, NULL);
	Log_write(1, "main start:%ld %ld\n", tv_bak.tv_sec, tv_bak.tv_usec);
	pthread_create(&pid1, NULL, thread_write1, NULL);
	pthread_create(&pid2, NULL, thread_write2, NULL);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	gettimeofday(&tv, NULL);
	printf("main exit:%ld %ld \n", tv.tv_sec - tv_bak.tv_sec, tv.tv_usec - tv_bak.tv_usec);
	struct stat stat_info;
	Log_close();
	char file_name[64] = { 0 };
	sprintf(file_name, "%s.%d", prefix, 0);
	stat(file_name, &stat_info);
	printf("%s: %ld\n", file_name, stat_info.st_blksize * stat_info.st_blocks);
	return 0;
}
