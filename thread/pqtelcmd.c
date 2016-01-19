/*
 ============================================================================
 Name        : pqtelcmd.c
 Author      : pqtel:zhoulp
 Version     :
 Copyright   : copyright all right reserved.
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include "file_log.h"
#include "nvr_base.h"
#include "pqtelcmd.h"
#include "readmediaframe.h"
#define SERVER_PORT2VENC_CTL 8003
#define SERVER_PORT2VENC_VIDEO 8004
//#define SERVER_PORT2VENC 8003
#define SERVER_PORT2LDP_HEART 7001
#define SERVER_PORT2LDP_VIDEO 7002
#define LENGTH_VENC_DATA 18
#define LENGTH_VIDEO_DATA_BUFFER  (65536+6)     //根据1. AV1/AV2发送给AV3的视频流报文格式 标示视频长度的才两个字节， 因此最大长度可以确定
static int sockfd_venc_ctl; /* socket descriptors */
static int sockfd_venc_video; /* socket descriptors */
static int sockfd_ldp_heart; /* socket descriptors */
static int sockfd_ldp_video;

extern systemparam sysconfig;
extern encoderparam av[2];
extern channelparam channel_gbk[2][8];
static struct sockaddr_in _6A_AV1_Addr;
static struct sockaddr_in _6A_AV2_Addr;
static struct sockaddr_in _6A_LDP_Addr;//这个地址不能写死，1 要么根据收到的心跳消息来，2要么根据配置文件来
static struct sockaddr_in _6A_LDP_NULLIP;

static int stop = 0;
static pthread_mutex_t stop_lock = PTHREAD_MUTEX_INITIALIZER;
static int history_thread_run = 0;
static pthread_t recv_tid;
static pthread_t heart_tid;
static pthread_t vod_tid;
static pthread_t vod_heart_tid;
//static sem_t sem_stop;
//static pthread_t re_tid;
//static time_t heart_send;
//static time_t heart_recv_av1;
//static time_t heart_recv_av2;
void* heart_6ACentral(void *p)
{
	pthread_detach(pthread_self());
	//struct sockaddr_in client; /* client's address information */
	char heart_data[LENGTH_VENC_DATA] = { 0 };
	memset(heart_data, 0x00, sizeof(heart_data));
	heart_data[0] = 0xBB;
	heart_data[1] = 0xBB;
	heart_data[2] = 0x12;//报文长度
	heart_data[3] = 0x00;//报文长度
	heart_data[4] = 0x04;// 报文类型:0x04=心跳(AV3 每10 秒发送一次,AV1/AV2 接收后立即返回,30 秒收不到心跳断开连接)

	heart_data[5] = 0x01;//应答类型:0x00=不需应答(AV1/AV2->AV3),0x01=需要应答(AV3->AV1/AV2)
	heart_data[6] = 0x03;//AV3
	heart_data[7] = 0x01;// AV1=0x01, AV2 = 0x02
	heart_data[16] = 0x00;//normal
	byte crc1 = 0x00, crc2 = 0x00;
	int i = 0;
	heart_data[sizeof(heart_data) - 1] = 0x00;
	for(i = 0; i < sizeof(heart_data) - 1; i++)
	{
		crc1 += heart_data[i];
	}

	heart_data[7] = 0x02;// AV1=0x01, AV2 = 0x02
	heart_data[sizeof(heart_data) - 1] = 0x00;
	for(i = 0; i < sizeof(heart_data) - 1; i++)
	{
		crc2 += heart_data[i];
	}
	int res1 = 0, res2 = 0;
	do{
		heart_data[7] = 0x01;// AV1=0x01, AV2 = 0x02
		heart_data[sizeof(heart_data) - 1] = crc1;
		res1 = sendto(sockfd_venc_ctl, (char*)heart_data, sizeof(heart_data), 0, (struct sockaddr *)&_6A_AV1_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_DEBUG, "heart_6ACentral sendto_6A_AV1_Addr[%s:%d]res1 = %d LENGTH_VENC_DATA[%02hx%02hx%02hx%02hx"
				" %02hx%02hx%02hx%02hx \n"
				"%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx %02hx%02hx]\n",
				inet_ntoa(_6A_AV1_Addr.sin_addr), ntohs(_6A_AV1_Addr.sin_port), res1,
				heart_data[0], heart_data[1], heart_data[2], heart_data[3],
				heart_data[4], heart_data[5], heart_data[6], heart_data[7],
				heart_data[8], heart_data[9], heart_data[10], heart_data[11],
				heart_data[12], heart_data[13], heart_data[14], heart_data[15], heart_data[16], heart_data[17]);

		heart_data[7] = 0x02;// AV1=0x01, AV2 = 0x02
		heart_data[sizeof(heart_data) - 1] = crc2;
		res2 = sendto(sockfd_venc_ctl, (char*)heart_data, sizeof(heart_data), 0, (struct sockaddr *)&_6A_AV2_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_DEBUG, "heart_6ACentral sendto_6A_AV2_Addr[%s:%d]res2 = %d LENGTH_VENC_DATA[%02hx%02hx%02hx%02hx"
				" %02hx%02hx%02hx%02hx \n"
				"%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx %02hx%02hx]\n",
				inet_ntoa(_6A_AV2_Addr.sin_addr), ntohs(_6A_AV2_Addr.sin_port), res2,
				heart_data[0], heart_data[1], heart_data[2], heart_data[3],
				heart_data[4], heart_data[5], heart_data[6], heart_data[7],
				heart_data[8], heart_data[9], heart_data[10], heart_data[11],
				heart_data[12], heart_data[13], heart_data[14], heart_data[15], heart_data[16], heart_data[17]);

		sleep(10);
	}while(true);
}
/*
 * 根据av1 av2的反馈， 判断心跳情况， 及时回复给LDP 请求结果。
 */
void* recv_6ACentral(void *p)
{
	pthread_detach(pthread_self());
	char recvmsg[LENGTH_VENC_DATA] = { 0 };
	assert(sizeof(VIDEOPLAY) == 33);
	char send_msg[sizeof(VIDEOPLAY)] = { 0 };

	struct sockaddr_in client; /* client's address information */
	socklen_t sin_size;
	int num = 0, i = 0;
	sin_size = sizeof(struct sockaddr_in);
	while(true) {
		// 接收请求
		LOG_TFL(LOG_DEBUG, "recv_6ACentral before recvfrom\n");
		num = recvfrom(sockfd_venc_ctl, recvmsg, sizeof(recvmsg), 0, (struct sockaddr *)&client, &sin_size);
		LOG_TFL(LOG_INFO, "recv_6ACentral recvfrom[%s:%d] num = %d\n LENGTH_VENC_DATA[%02hx%02hx%02hx%02hx"
				" %02hx%02hx%02hx%02hx \n"
				"%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx %02hx%02hx]\n",
				inet_ntoa(client.sin_addr), ntohs(client.sin_port), num,
				recvmsg[0], recvmsg[1], recvmsg[2], recvmsg[3],
				recvmsg[4], recvmsg[5], recvmsg[6], recvmsg[7],
				recvmsg[8], recvmsg[9], recvmsg[10], recvmsg[11],
				recvmsg[12], recvmsg[13], recvmsg[14], recvmsg[15], recvmsg[16], recvmsg[17]);

		if(num == LENGTH_VENC_DATA && recvmsg[0] == 0xBB && recvmsg[1] == 0xBB
				&& recvmsg[2] == 0x12 && recvmsg[3] == 0x00 && recvmsg[5] == 0x00 && recvmsg[7] == 0x03
				&& (recvmsg[6] == 0x01 || recvmsg[6] == 0x02))
		{
			memset(send_msg, 0x00, sizeof(send_msg));
			VIDEOPLAY *videoplay_tmp = (VIDEOPLAY *)send_msg;
			videoplay_tmp->head[0] = 0xAA;
			videoplay_tmp->head[1] = 0xAA;
			videoplay_tmp->flag = 0x57;
			videoplay_tmp->command = 0x01;
			if(recvmsg[6] == 0x01)
			{
				videoplay_tmp->channel[0] = recvmsg[8];
				videoplay_tmp->channel[1] = recvmsg[9];
				videoplay_tmp->channel[2] = recvmsg[10];
				videoplay_tmp->channel[3] = recvmsg[11];

				videoplay_tmp->channel[4] = recvmsg[12];
				videoplay_tmp->channel[5] = recvmsg[13];
				videoplay_tmp->channel[6] = recvmsg[14];
				videoplay_tmp->channel[7] = recvmsg[15];
			}else
			{
				videoplay_tmp->channel[8] = recvmsg[8];
				videoplay_tmp->channel[9] = recvmsg[9];
				videoplay_tmp->channel[10] = recvmsg[10];
				videoplay_tmp->channel[11] = recvmsg[11];

				videoplay_tmp->channel[12] = recvmsg[12];
				videoplay_tmp->channel[13] = recvmsg[13];
				videoplay_tmp->channel[14] = recvmsg[14];
				videoplay_tmp->channel[15] = recvmsg[15];
			}
			switch(recvmsg[4]){

				case 0x01:
				{//0x01=请求视频流
					//videoplay_tmp->respond = 0x01;
					//videoplay_tmp->res_type = 0x01;
				}
				break;
				case 0x02:
				{//0x02=结束视频流
					//由于广义停止视频播放的时候，会停止两块编码版所有通道，因此这里不再回复停止消息，
					//根据北京测试要求，av3收到请求停止消息，在stopPlayVideo函数立即回复就好了
					//videoplay_tmp->respond = 0x02;
					//videoplay_tmp->res_type = 0x01;

				}
				break;
				case 0x03:
				{//0x03=视频截图

				}
				break;
				case 0x04:
				{//0x04=心跳

				}
				break;
				default:
				break;
			}
			if(videoplay_tmp->respond > 0){
				for(i = 0; i < sizeof(send_msg) - 1; i++)
				{
					videoplay_tmp->check += send_msg[i];
				}

				sendto(sockfd_ldp_video, send_msg, sizeof(send_msg), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
			}
		}

		//TODO LDP的数据接收端口， 在协议中没有明确规定
		//DATETIME数据结构体， 不太明确
		//sendto(sockfd_ldp_heart, (char*)recvmsg, num, 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
	}
}
//为了简易实现功能，没做互斥，实际代码必须做互斥处理！！！！！
unsigned char cal_crc(unsigned char *msg)
{
	int i;
	unsigned char crc=0;
	for(i=0;i<17;i++)
	{
		crc+=msg[i];
	}
	return crc;
}
bool crcIsRight(byte *msg, unsigned int size)
{
	if(size < 2 || msg == NULL)
		return false;

	byte crc = 0;
	int i = 0;
	for(i = 0; i < size - 1; i++)
	{
		crc += msg[i];
	}
	return crc == msg[size -1];
}
void reqPlayVideo(VIDEOPLAY *videoplay)
{
	char play_data[LENGTH_VENC_DATA] = { 0 };
	get_encoder_db();
	update_channel_db();
	play_data[0] = 0xBB;
	play_data[1] = 0xBB;
	play_data[2] = 0x12;
	play_data[3] = 0x00;
	play_data[4] = 0x01;// 0x01=请求视频流,0x02=结束视频流,0x03=视频截图,
	play_data[5] = 0x01;//应答类型:0x00=不需应答(AV1/AV2->AV3),0x01=需要应答(AV3->AV1/AV2)
	play_data[6] = 0x03;
	play_data[16] = 0x00;
	int i = 0;
	int res = 0;
	bool send_flag = false;
	int send_count = 0;
	for(i = 0; i < 8; i++)
	{
		if(videoplay->channel[i] && av[0].VENC1_VALID && channel_gbk[0][i].VENC1_CH01_VALID)
		{
			send_flag = true;
			play_data[8 + i] = 0x01;
			LOG_TFL(LOG_INFO, "reqPlayVideo AV1 channel is = %d\n", i);
		}
	}
	if(send_flag){
		send_count++;
		play_data[7] = 0x01;// AV1=0x01, AV2 = 0x02
		play_data[sizeof(play_data) - 1] = 0x00;
		for(i = 0; i < sizeof(play_data) - 1; i++)
		{
			play_data[sizeof(play_data) - 1] += play_data[i];
		}
		int res = sendto(sockfd_venc_ctl, (char*)play_data, sizeof(play_data), 0, (struct sockaddr *)&_6A_AV1_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_INFO, "reqPlayVideo _6A_AV1_Addr[%s:%d] num = %d\n LENGTH_VENC_DATA[%02hx%02hx%02hx%02hx"
			" %02hx%02hx%02hx%02hx \n"
			"%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx %02hx%02hx]\n",
			inet_ntoa(_6A_AV1_Addr.sin_addr), ntohs(_6A_AV1_Addr.sin_port), res,
			play_data[0], play_data[1], play_data[2], play_data[3],
			play_data[4], play_data[5], play_data[6], play_data[7],
			play_data[8], play_data[9], play_data[10], play_data[11],
			play_data[12], play_data[13], play_data[14], play_data[15], play_data[16], play_data[17]);
	}
	send_flag = false;
	for(i = 8; i < 16; i++)
	{
		if(videoplay->channel[i] && av[1].VENC1_VALID && channel_gbk[1][i - 8].VENC1_CH01_VALID)
		{
			send_flag = true;
			play_data[8 + i - 8] = 0x01;
			LOG_TFL(LOG_INFO, "reqPlayVideo AV2 channel is = %d\n", i - 8);
		}
	}
	if(send_flag){
		send_count++;
		play_data[7] = 0x02;// AV1=0x01, AV2 = 0x02
		play_data[sizeof(play_data) - 1] = 0x00;
		for(i = 0; i < sizeof(play_data) - 1; i++)
		{
			play_data[sizeof(play_data) - 1] += play_data[i];
		}
		res = sendto(sockfd_venc_ctl, (char*)play_data, sizeof(play_data), 0, (struct sockaddr *)&_6A_AV2_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_INFO, "reqPlayVideo _6A_AV2_Addr[%s:%d] num = %d\n LENGTH_VENC_DATA[%02hx%02hx%02hx%02hx"
			" %02hx%02hx%02hx%02hx \n"
			"%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx %02hx%02hx]\n",
			inet_ntoa(_6A_AV2_Addr.sin_addr), ntohs(_6A_AV2_Addr.sin_port), res,
			play_data[0], play_data[1], play_data[2], play_data[3],
			play_data[4], play_data[5], play_data[6], play_data[7],
			play_data[8], play_data[9], play_data[10], play_data[11],
			play_data[12], play_data[13], play_data[14], play_data[15], play_data[16], play_data[17]);
	}
	VIDEOPLAY videoplay_resp;
	if(0 == send_count){


		memset(&videoplay_resp, 0x00, sizeof(videoplay_resp));
		videoplay_resp.head[0] = 0xAA;
		videoplay_resp.head[1] = 0xAA;
		videoplay_resp.flag = 0x57;
		videoplay_resp.command = 0x01;

		videoplay_resp.respond = 0x04;//没有码流
		videoplay_resp.res_type = 0x01;
		videoplay_resp.check = 0x00;

		videoplay_resp.check = videoplay_resp.head[0]
						+ videoplay_resp.head[1]
					    + videoplay_resp.flag
						+ videoplay_resp.command
						+ videoplay_resp.respond
						+ videoplay_resp.res_type;
		sendto(sockfd_ldp_video, &videoplay_resp, sizeof(VIDEOPLAY), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
	}else
	{
		memset(&videoplay_resp, 0x00, sizeof(videoplay_resp));
		videoplay_resp.head[0] = 0xAA;
		videoplay_resp.head[1] = 0xAA;
		videoplay_resp.flag = 0x57;
		videoplay_resp.command = 0x01;

		videoplay_resp.respond = 0x01;//传输开始
		videoplay_resp.res_type = 0x01;
		videoplay_resp.check = 0x00;

		videoplay_resp.check = videoplay_resp.head[0]
						+ videoplay_resp.head[1]
					    + videoplay_resp.flag
						+ videoplay_resp.command
						+ videoplay_resp.respond
						+ videoplay_resp.res_type;
		sendto(sockfd_ldp_video, &videoplay_resp, sizeof(VIDEOPLAY), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
	}

}
void stopPlayVideo(VIDEOPLAY *videoplay)
{
	char play_data[LENGTH_VENC_DATA] = { 0 };
	play_data[0] = 0xBB;
	play_data[1] = 0xBB;
	play_data[2] = 0x12;
	play_data[3] = 0x00;
	play_data[4] = 0x02;// 0x01=请求视频流,0x02=结束视频流,0x03=视频截图,
	play_data[5] = 0x01;//应答类型:0x00=不需应答(AV1/AV2->AV3),0x01=需要应答(AV3->AV1/AV2)
	play_data[6] = 0x03;
	play_data[16] = 0x00;
	int i = 0;
	int res = 0;
	bool send_flag = false;
	for(i = 0; i < 7; i++)
	{
		if(videoplay->channel[i])
		{
			send_flag = true;
			//play_data[8 + i] = 0x01;
			LOG_TFL(LOG_INFO, "stopPlayVideo AV1 channel is = %d\n", i);
		}
	}
	if(send_flag){
		play_data[7] = 0x01;// AV1=0x01, AV2 = 0x02
		play_data[sizeof(play_data) - 1] = 0x00;
		for(i = 0; i < sizeof(play_data) - 1; i++)
		{
			play_data[sizeof(play_data) - 1] += play_data[i];
		}
		//
		res = sendto(sockfd_venc_ctl, play_data, sizeof(play_data), 0, (struct sockaddr *)&_6A_AV1_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_INFO, "stopPlayVideo _6A_AV1_Addr[%s:%d] num = %d\n LENGTH_VENC_DATA[%02hx%02hx%02hx%02hx"
			" %02hx%02hx%02hx%02hx \n"
			"%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx %02hx%02hx]\n",
			inet_ntoa(_6A_AV1_Addr.sin_addr), ntohs(_6A_AV1_Addr.sin_port), res,
			play_data[0], play_data[1], play_data[2], play_data[3],
			play_data[4], play_data[5], play_data[6], play_data[7],
			play_data[8], play_data[9], play_data[10], play_data[11],
			play_data[12], play_data[13], play_data[14], play_data[15], play_data[16], play_data[17]);
		//sendto(sockfd_ldp_video, &videoplay_stop, sizeof(VIDEOPLAY), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
	}

	send_flag = false;
	for(i = 8; i < 15; i++)
	{
		if(videoplay->channel[i])
		{
			send_flag = true;
			//play_data[8 + i - 8] = 0x01;
			LOG_TFL(LOG_INFO, "stopPlayVideo AV2 channel is = %d\n", i-8);
		}
	}
	if(send_flag){
		play_data[7] = 0x02;// AV1=0x01, AV2 = 0x02
		play_data[sizeof(play_data) - 1] = 0x00;
		for(i = 0; i < sizeof(play_data) - 1; i++)
		{
			play_data[sizeof(play_data) - 1] += play_data[i];
		}
		res = sendto(sockfd_venc_ctl, play_data, sizeof(play_data), 0, (struct sockaddr *)&_6A_AV2_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_INFO, "stopPlayVideo _6A_AV2_Addr[%s:%d] num = %d\n LENGTH_VENC_DATA[%02hx%02hx%02hx%02hx"
			" %02hx%02hx%02hx%02hx \n"
			"%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx %02hx%02hx]\n",
			inet_ntoa(_6A_AV2_Addr.sin_addr), ntohs(_6A_AV2_Addr.sin_port), res,
			play_data[0], play_data[1], play_data[2], play_data[3],
			play_data[4], play_data[5], play_data[6], play_data[7],
			play_data[8], play_data[9], play_data[10], play_data[11],
			play_data[12], play_data[13], play_data[14], play_data[15], play_data[16], play_data[17]);
		//sendto(sockfd_ldp_video, &videoplay_stop, sizeof(VIDEOPLAY), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
	}
	// win AV3上抓包， 停止视频的包为 bb:bb:12:00:02:00:03:01:00:00:00:00:00:00:00:00:00:8e (不需回应， 不需写通道号)
	VIDEOPLAY videoplay_stop_resp;
	memset(&videoplay_stop_resp, 0x00, sizeof(videoplay_stop_resp));
	videoplay_stop_resp.head[0] = 0xAA;
	videoplay_stop_resp.head[1] = 0xAA;
	videoplay_stop_resp.flag = 0x57;
	videoplay_stop_resp.command = 0x01;

	videoplay_stop_resp.respond = 0x02;
	videoplay_stop_resp.res_type = 0x01;
	videoplay_stop_resp.check = 0x00;

	videoplay_stop_resp.check = videoplay_stop_resp.head[0]
					+ videoplay_stop_resp.head[1]
				    + videoplay_stop_resp.flag
					+ videoplay_stop_resp.command
					+ videoplay_stop_resp.respond
					+ videoplay_stop_resp.res_type;
	sendto(sockfd_ldp_video, &videoplay_stop_resp, sizeof(VIDEOPLAY), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));

}
/*
 * 历史视频文件查找读取，发送功能
 */
void* reSendFile(void *p)
{
	pthread_detach(pthread_self());

	//struct timespec abs_timeout = {0, 0};
	//struct timeval abs_time = {0, 0};
	int i = 0, timespan = 0, j = 0;
	int ret = 0, flag = 0;
	VIDEOPLAY videoplay_sea;
	memcpy(&videoplay_sea, p, sizeof(videoplay_sea));
	char send_msg[sizeof(VIDEOPLAY)] = { 0 };
	memset(send_msg, 0x00, sizeof(send_msg));
	VIDEOPLAY *videoplay_tmp = (VIDEOPLAY *)send_msg;
	videoplay_tmp->head[0] = 0xAA;
	videoplay_tmp->head[1] = 0xAA;
	videoplay_tmp->flag = 0x57;
	videoplay_tmp->command = 0x02;
	memcpy(videoplay_tmp->channel, videoplay_sea.channel, 16);

	videoplay_tmp->respond = 0x01;
	videoplay_tmp->res_type = 0x02;

	videoplay_tmp->check = 0x00;
	for(i = 0; i < sizeof(send_msg) - 1; i++)
	{
		videoplay_tmp->check += send_msg[i];
	}

	//read(b);

	int chans[16] = { 0 }, chan_num = 0, result_num = 0;
	stream_list *result_list = NULL;
	tms start_time = {
				videoplay_sea.timestart[5],
				videoplay_sea.timestart[4],
				videoplay_sea.timestart[3],
				videoplay_sea.timestart[2],
				videoplay_sea.timestart[1],
				videoplay_sea.timestart[0]};

	tms end_time;
	struct tm start_tm = {0 }, end_tm = { 0 };
	start_tm.tm_year = start_time.year + 100;
	start_tm.tm_mon = start_time.month - 1;
	start_tm.tm_mday = start_time.day;
	start_tm.tm_hour = start_time.hour;
	start_tm.tm_min = start_time.minute;
	start_tm.tm_sec = start_time.second;
	time_t start_t = mktime(&start_tm);
	timespan = videoplay_sea.timespan[1]*256+videoplay_sea.timespan[0];
	//历史视频流点播限制在30-300秒之间；
	if(timespan > 300) timespan = 300;
	if(timespan < 30) timespan = 30;
	LOG_TFL(LOG_DEBUG, "reqFileVideo start_time [%04d-%02d-%02d %02d:%02d:%02d] timespan = %d start_t = %d\n",
				start_tm.tm_year + 1900, start_tm.tm_mon + 1, start_tm.tm_mday, start_tm.tm_hour, start_tm.tm_min, start_tm.tm_sec
			, timespan, start_t);

	time_t end_t = start_t + timespan;
	localtime_r(&end_t, &end_tm);
	LOG_TFL(LOG_DEBUG, "reqFileVideo start_time [%04d-%02d-%02d %02d:%02d:%02d] timespan = %d end_t = %d\n",
				end_tm.tm_year + 1900, end_tm.tm_mon + 1, end_tm.tm_mday, end_tm.tm_hour, end_tm.tm_min, end_tm.tm_sec, timespan, end_t);
	end_time.year = end_tm.tm_year - 100;
	end_time.month = end_tm.tm_mon + 1;
	end_time.day = end_tm.tm_mday;
	end_time.hour = end_tm.tm_hour;
	end_time.minute = end_tm.tm_min;
	end_time.second = end_tm.tm_sec;
	for(i = 0; i < 16; i++)
	{
		if(videoplay_sea.channel[i] && i != 7 && i != 15)
		{
			chans[chan_num] = i + 1;
			chan_num++;
			break;//历史视频只需要支持一个通道点播
			LOG_TFL(LOG_DEBUG, "reqFileVideo chan_num = %d, chan is %d\n", chan_num, chans[chan_num]);
		}
	}


	ret = findrecord_noidx(chans, chan_num, start_time, end_time, &result_list, &result_num);
	LOG_TFL(LOG_INFO, "findrecord_noidx ret = %d, result_num = %d\n", ret, result_num);
	byte video_data[LENGTH_VIDEO_DATA_BUFFER + 2] = { 0 };
	video_data[0] = 0xAA;
	video_data[1] = 0xAA;
	video_data[4] = 0x58;//flag
	video_data[5] = 0x02;//报文类型：0x01 实时视频 0x02
	int video_data_send = 0;
	int handle = 0;
	if(result_num > 0)
	{
		//先回复点播历史文件成功
		sendto(sockfd_ldp_video, send_msg, sizeof(send_msg), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_INFO, "sockfd_ldp_video1 ret = %d \n", ret);
	}
	for(j = 0; j < result_num; j++, result_list++)
	{
		char date_file[256] = { 0 };
		sprintf(date_file, "%s/20%02d-%02d-%02d/%s", sysconfig.RecordingPath, result_list->starttime.year, result_list->starttime.month, result_list->starttime.day, result_list->record_filename);

		int len = 0;
		int file_startsec = 0, file_endsec = 0;
		int frametype= -1;
		unsigned int timestamp = -1;
		handle = openmedia(date_file);
		if(handle == 0)
		{
			LOG_TFL(LOG_ERROR, "openmedia handle = %d \n", handle);
		}
		int duration = getduration(handle);
		LOG_TFL(LOG_INFO, "date_file[%s]getduration duration = %d \n", date_file, duration);
		struct tm file_tm = { 0 };
		file_tm.tm_year = result_list->starttime.year + 100;
		file_tm.tm_mon = result_list->starttime.month - 1;
		file_tm.tm_mday = result_list->starttime.day;
		file_tm.tm_hour = result_list->starttime.hour;
		file_tm.tm_min = result_list->starttime.minute;
		file_tm.tm_sec = result_list->starttime.second;
		file_startsec =  mktime(&file_tm);
		if(j == 0 && file_startsec > start_t)
		{// 如果是第一个文件, start_t 点播开始时间应该大于等于第一个文件的开始时间
			LOG_TFL(LOG_ERROR, "first file start_t < file_startsec\n", ret);
			break;
		}
		if(file_startsec > end_t)
		{// 文件开始时间，大于查询结束时间则认为，这个文件可以不处理。
			LOG_TFL(LOG_WARN, "first file_startsec > end_t, break for\n", ret);
			break;
		}
		if(start_t - file_startsec > 0){
			ret = set_clip_time(handle, start_t - file_startsec, duration);
		}
		else
		{
			ret = set_clip_time(handle, 0, duration);
		}
		LOG_TFL(LOG_INFO, "set_clip_time ret = %d , file_startsec = %d, start_t - file_startsec = %d\n", ret, file_startsec, start_t - file_startsec);
		int count_log = 0;
		do{
			ret = readnextframe(handle, video_data + 7, &len, &frametype, &timestamp);
			video_data_send = len + 8;
			video_data[2] = video_data_send;//报文长度
			video_data[3] = video_data_send>>8;//报文长度
			video_data[6] = result_list->chan;//通道号【01-16】08、16通道为音频通道不使用
			if(count_log % 10 == 0)
			{
				count_log = 0;
				LOG_TFL(LOG_DEBUG, "readnextframe start_t = %d, file_startsec = %d, timestamp = %d,  (int )(timestamp/1000) = %d\n",
						start_t, file_startsec, timestamp, (int )(timestamp/1000));
			}
			if(ret < 0)
			{
				LOG_TFL(LOG_INFO, "readnextframe end ret = %d err2\n", ret);
			    break;
			}
			if(file_startsec + (int )(timestamp/1000) - start_t > timespan)
			{
				break;
			}
			video_data[video_data_send - 1] = 0x00;
			for(i = 0; i < video_data_send - 1; i++)
			{
				video_data[video_data_send - 1] += video_data[i];
			}
			//usleep(10000);
			//TODO LDP的数据接收端口， 在协议中没有明确规定
			//DATETIME数据结构体， 不太明确
			if(ret == 0){
				ret = sendto(sockfd_ldp_video, (char*)video_data, video_data_send, 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
				//LOG_TFL(LOG_INFO, "sendto ret = %d abs_timeout.tv_sec = %ld, %ld\n", ret, abs_timeout.tv_sec, abs_timeout.tv_nsec);
			}

			//CLOCK_REALTIME(CLOCK_REALTIME, &abs_timeout);
			/*
			gettimeofday(&abs_time, NULL);
			if(abs_time.tv_usec + 100 > 999999)
			{
				//abs_timeout.tv_sec = abs_time.tv_sec + 1;
				//abs_timeout.tv_nsec = (abs_time.tv_usec + 10000 - 1000000) * 1000;
				//abs_timeout.tv_nsec = (abs_time.tv_usec - 999900) * 1000;
				LOG_TFL(LOG_INFO, "sem_timedwait next second %ld, %ld\n", abs_timeout.tv_sec, abs_timeout.tv_nsec);
			}else
			{
				abs_timeout.tv_sec = abs_time.tv_sec;
				abs_timeout.tv_nsec = (abs_time.tv_usec + 100) * 1000;
			}
			ret = sem_timedwait(&sem_stop, &abs_timeout);
			int value = -1;
			sem_getvalue(&sem_stop, &value);
			if(value > 0)LOG_TFL(LOG_INFO, "sem_timedwait sem_getvalue = %d\n", value);
			//LOG_TFL(LOG_INFO, "sem_timedwait ret = %d \n", ret);
			if(ret == 0)
			{
				LOG_TFL(LOG_INFO, "history filesend thread reveived stop signal \n");
				goto GOTO_EXIT;
			}
			*/

			if(stop == 1)
			{
				pthread_mutex_lock(&stop_lock);
				stop = 0;
				history_thread_run = 0;
				pthread_mutex_unlock(&stop_lock);
				goto GOTO_EXIT;
			}

			usleep(10);
			//memcpy(video_data+7, recvmsg+5, video_data_len);
		}while(true);
		if(handle){
			closemedia(handle);
			handle = 0;
		}
		LOG_TFL(LOG_INFO, "nextfor result_num = %d\n", result_num);
	}
GOTO_EXIT:
	history_thread_run = 0;
	if(handle){
		closemedia(handle);
		handle = 0;
	}
	videoplay_tmp->respond = 0x02;

	videoplay_tmp->res_type = 0x02;

	videoplay_tmp->check = 0x00;
	//正常结束或者没有历史视频文件
	if(result_num == 0 || j == result_num){
		if(result_num == 0)videoplay_tmp->respond = 0x05;
		for(i = 0; i < sizeof(send_msg) - 1; i++)
		{
			videoplay_tmp->check += send_msg[i];
		}
		ret = sendto(sockfd_ldp_video, send_msg, sizeof(send_msg), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_INFO, "sockfd_ldp_video2 ret = %d \n", ret);
	}

	return NULL;
}

int reqFileVideo(VIDEOPLAY *videoplay)
{
	pthread_t tid;
	return pthread_create(&tid, NULL, reSendFile, videoplay);

}
/*
 * 视频点播服务线程
 */
void* videoOnDemandHeart(void *p)
{
	// heart ldp:5555 --udp------> av3:7001  回复心跳  ldp:5555 --udp------> av3:7001
	// request ldp:7002 --udp------> av3:7002
	pthread_detach(pthread_self());
	struct sockaddr_in client; /* client's address information */
	socklen_t sin_size;
	int num = 0;
	char resp_video[sizeof(VIDEOINFO)] = { 0 };
	VIDEOINFO *resp_video2ldp = (VIDEOINFO *)resp_video;
	assert(sizeof(VIDEOINFO) == 8);
	resp_video2ldp->head[0] = 0xaa;//[aaaa0800 560100b3]
	resp_video2ldp->head[1] = 0xaa;
	resp_video2ldp->size[0] = 0x08;
	resp_video2ldp->size[1] = 0x00;
	resp_video2ldp->flag = 0x56;
	resp_video2ldp->command = 0x02;
	resp_video2ldp->info = 0x01;

	resp_video2ldp->check = resp_video[0] + resp_video[1] + resp_video[2] + resp_video[3] + resp_video[4] + resp_video[5] + resp_video[6];
	LOG_TFL(LOG_INFO, "videoOnDemandHeart resp_video2ldp is [%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx]%02hx\n", resp_video[0], resp_video[1]
		, resp_video[2], resp_video[3], resp_video[4], resp_video[5], resp_video[6], resp_video[7]);
	char recvmsg[sizeof(VIDEOPLAY)] = { 0 };
	sin_size = sizeof(struct sockaddr_in);
	while(true) {
		// 接收请求

		num = recvfrom(sockfd_ldp_heart, recvmsg, sizeof(recvmsg), 0, (struct sockaddr *)&client, &sin_size);
		if(0 == memcmp(&_6A_LDP_Addr, &_6A_LDP_NULLIP, sizeof(_6A_LDP_NULLIP)))
		{
			_6A_LDP_Addr.sin_addr = client.sin_addr;
			LOG_TFL(LOG_INFO, "videoOnDemandHeart sockfd_ldp_heart recvfrom [%s:%d] num = %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), num);
		}
		if(num == sizeof(VIDEOINFO))
		{
			VIDEOINFO *videoinfo = (VIDEOINFO *)recvmsg;//[aaaa0800 560100b3]
			if(videoinfo->command == 0x01)
			LOG_TFL(LOG_DEBUG, "videoOnDemandHeart before recvfrom [%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx]%02hx\n", videoinfo->head[0], videoinfo->head[1]
				, videoinfo->size[0], videoinfo->size[1], videoinfo->flag, videoinfo->command, videoinfo->info, videoinfo->check,
				(char)(videoinfo->head[0]+ videoinfo->head[1]
				+ videoinfo->size[0]+ videoinfo->size[1]+ videoinfo->flag+ videoinfo->command+ videoinfo->info));

			sendto(sockfd_ldp_heart, resp_video2ldp, sizeof(VIDEOINFO), 0, (struct sockaddr *)&client, sizeof(struct sockaddr_in));
		}

	}
	return NULL;
}
/*
 * 视频点播服务线程
 */
void* videoOnDemand(void *p)
{
	// heart ldp:5555 --udp------> av3:7001  回复心跳  ldp:5555 --udp------> av3:7001
	// request ldp:7002 --udp------> av3:7002
	pthread_detach(pthread_self());
	struct sockaddr_in client; /* client's address information */
	socklen_t sin_size = sizeof(struct sockaddr_in);
	int num = 0;
	//int value = -1;
	char recvmsg[sizeof(VIDEOPLAY)] = { 0 };
	char send_msg[sizeof(VIDEOPLAY)] = { 0 };
	memset(send_msg, 0x00, sizeof(send_msg));
	VIDEOPLAY *videoplay_tmp = (VIDEOPLAY *)send_msg;
	videoplay_tmp->head[0] = 0xAA;
	videoplay_tmp->head[1] = 0xAA;
	videoplay_tmp->flag = 0x57;
	videoplay_tmp->command = 0x02;

	videoplay_tmp->respond = 0x02;
	videoplay_tmp->res_type = 0x02;

	videoplay_tmp->check = 0x00;
	int i = 0;
	for(i = 0; i < sizeof(send_msg) - 1; i++)
	{
		videoplay_tmp->check += send_msg[i];
	}
	LOG_TFL(LOG_DEBUG, "videoOn=%d\n", sizeof(VIDEOPLAY));
	while(true) {
		// 接收请求
		num = recvfrom(sockfd_ldp_video, recvmsg, sizeof(recvmsg), 0, (struct sockaddr *)&client, &sin_size);
		if(0 == memcmp(&_6A_LDP_Addr, &_6A_LDP_NULLIP, sizeof(_6A_LDP_NULLIP)))
		{//此处也要给_6A_LDP_Addr IP赋值，防止（这里有可能比心跳先执行，比如客户端心跳还在等待状态，就开始点播）
			//出现广播消息发给本机,导致sockfd_ldp_video 收到，形成无限循环
			_6A_LDP_Addr.sin_addr = client.sin_addr;
			LOG_TFL(LOG_INFO, "videoOnDemandHeart sockfd_ldp_heart recvfrom [%s:%d] num = %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), num);
		}
		LOG_TFL(LOG_DEBUG, "videoOnDemand sockfd_ldp_video recvfrom [%s:%d] num = %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), num);
		if(num == 33){

			int i = 0;
			char print_msg[66] = { 0 };
			for(i = 0; i < num; i++)
			{
				sprintf(print_msg+i*2, "%02hx", recvmsg[i]);
			}
			// 实时点播请求数据 aaaa210057010100000000000000000000000000000000000000000000000000ce
			// 历史点播请求数据 aaaa210057020000000000000000000000000000000000000f09110f2c383c00a6
			//aaaa210057020000000000000000000000000000000000000f09110f2c383c00a6
			//aaaa21005702 01000000 00000000 00000000 00000100 0000 0f0911 0f2c38 3c 00a8[15:09:17:15:44:56:60]

			// 广义 aaaa210057010100000000000000000000000000000000000000000000000000ce
			//     aaaa210057030101010101010101010101010101010100000000000000000000df
			// 株所 aaaa210057010100000000000000000000000000000000000000000000000000ce
			//     aaaa210057030100000000000000000000000000000000000000000000000000d0
			LOG_TFL(LOG_WARN, "VideoOnDemand recvfrom sockfd_ldp_video len = %d [%s]\n", num, print_msg);
			VIDEOPLAY *videoplay_tmp = (VIDEOPLAY *)recvmsg;
			//if(videoplay->head[0] == 0xAA && videoplay->head[1] == 0xAA && videoplay->size[1] == 0x1F && videoplay->flag == 0x57)
			if(videoplay_tmp->command == 0x01)
			{
				LOG_TFL(LOG_INFO, "VideoOnDemand reqPlayVideo\n");
				reqPlayVideo(videoplay_tmp);
			}
			if(videoplay_tmp->command == 0x03)
			{
				LOG_TFL(LOG_INFO, "VideoOnDemand stopPlayVideo\n");
				stopPlayVideo(videoplay_tmp);
			}
			if(videoplay_tmp->command == 0x02)
			{
				//sem_destroy(&sem_stop);
				//sem_init(&sem_stop, 0, 0);

				//sem_getvalue(&sem_stop, &value);
				LOG_TFL(LOG_INFO, "VideoOnDemand reqFileVideo value = 0\n");
				pthread_mutex_lock(&stop_lock);
				if(history_thread_run == 0){
					stop = 0;
					(0 == reqFileVideo(videoplay_tmp))?(history_thread_run = 1):(history_thread_run = 0);
				}else
				{
					videoplay_tmp->respond = 0x01;
					sendto(sockfd_ldp_video, send_msg, sizeof(send_msg), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
				}
				pthread_mutex_unlock(&stop_lock);
			}
			if(videoplay_tmp->command == 0x04)
			{
				LOG_TFL(LOG_INFO, "VideoOnDemand try stopFileVideo\n");
				// 不管是否正在下载历史视频数据， 单点的时候都要回复消息
				videoplay_tmp->respond = 0x02;
				sendto(sockfd_ldp_video, send_msg, sizeof(send_msg), 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
				//sem_getvalue(&sem_stop, &value);
				LOG_TFL(LOG_INFO, "stopFileVideo sem_getvalue = 0\n");
				pthread_mutex_lock(&stop_lock);
				if(history_thread_run == 1)
				{
					//history_thread_run = 0;
					stop = 1;
					LOG_TFL(LOG_INFO, "VideoOnDemand sem_post stopFileVideo\n");
					//sem_post(&sem_stop);

					//sem_getvalue(&sem_stop, &value);
					//LOG_TFL(LOG_INFO, "sem_post sem_getvalue = %d\n", value);
				}
				pthread_mutex_unlock(&stop_lock);
			}
			//sendto(sockfd_ldp_video, (char*)recvmsg, num, 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
		}
//		}else
//		{
//			LOG_TFL(LOG_WARN, "VideoOnDemand recvfrom error data\n");
//		}
	}
	return NULL;
}

void initSet(){
	int value = -1;
	//sem_init(&sem_stop, 0, 0);
	//sem_getvalue(&sem_stop, &value);
	LOG_TFL(LOG_INFO, "sem_init sem_getvalue = %d\n", value);
	if(load_config_paramdb_gbk2utf8())
	{
		exit(EXIT_FAILURE);
	}
	LOG_TFL(LOG_INFO, "VENC1_IPADDR[%s], VENC2_IPADDR[%s]\n", av[0].VENC1_IPADDR, av[1].VENC1_IPADDR);
	bzero(&_6A_AV1_Addr, sizeof(struct sockaddr_in));
	_6A_AV1_Addr.sin_family = AF_INET;
	_6A_AV1_Addr.sin_port = htons(SERVER_PORT2VENC_CTL);
	inet_aton(av[0].VENC1_IPADDR, &_6A_AV1_Addr.sin_addr);


	bzero(&_6A_AV2_Addr, sizeof(struct sockaddr_in));
	_6A_AV2_Addr.sin_family = AF_INET;
	_6A_AV2_Addr.sin_port = htons(SERVER_PORT2VENC_CTL);
	inet_aton(av[1].VENC1_IPADDR, &_6A_AV2_Addr.sin_addr);


	bzero(&_6A_LDP_Addr, sizeof(struct sockaddr_in));
	_6A_LDP_Addr.sin_family = AF_INET;
	_6A_LDP_Addr.sin_port = htons(sysconfig.PORT_LDP);
	//inet_aton(sysconfig.IP_LDP, &_6A_LDP_Addr.sin_addr);
	_6A_LDP_Addr.sin_port = htons(SERVER_PORT2LDP_VIDEO);
	//inet_aton("192.168.60.25", &_6A_LDP_Addr.sin_addr);//这个地址不能写死，1 要么根据收到的心跳消息来，2要么根据配置文件来

	bzero(&_6A_LDP_NULLIP, sizeof(struct sockaddr_in));
	_6A_LDP_NULLIP.sin_family = AF_INET;
	_6A_LDP_NULLIP.sin_port = htons(sysconfig.PORT_LDP);
	//inet_aton(sysconfig.IP_LDP, &_6A_LDP_Addr.sin_addr);
	_6A_LDP_NULLIP.sin_port = htons(SERVER_PORT2LDP_VIDEO);

	struct sockaddr_in server2venc_ctl; /* server's address information */
	/* Creating UDP socket */
	if ((sockfd_venc_ctl = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		/* handle exception */
		perror("server2venc_ctl Creating socket failed.");
		LOG_TFL(LOG_FATAL, "server2venc_ctl Creating socket failed.");
		exit(EXIT_FAILURE);
	}
	bzero(&server2venc_ctl,sizeof(server2venc_ctl));
	server2venc_ctl.sin_family= AF_INET;

	server2venc_ctl.sin_port = htons(SERVER_PORT2VENC_CTL);
	server2venc_ctl.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd_venc_ctl, (struct sockaddr *)&server2venc_ctl, sizeof(struct sockaddr)) == -1) {
		/* handle exception */
		perror("server2venc_ctl Bind error.");
		LOG_TFL(LOG_FATAL, "server2venc_ctl Creating socket failed.");
		exit(EXIT_FAILURE);
	}


	struct sockaddr_in server2venc_video; /* server's address information */
	/* Creating UDP socket */
	if ((sockfd_venc_video = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		/* handle exception */
		perror("server2venc_video Creating socket failed.");
		LOG_TFL(LOG_FATAL, "server2venc_video Creating socket failed.");
		exit(EXIT_FAILURE);
	}
	bzero(&server2venc_video,sizeof(server2venc_video));
	server2venc_video.sin_family= AF_INET;

	server2venc_video.sin_port = htons(SERVER_PORT2VENC_VIDEO);
	server2venc_video.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd_venc_video, (struct sockaddr *)&server2venc_video, sizeof(struct sockaddr)) == -1) {
		/* handle exception */
		perror("server2venc_video Bind error.");
		LOG_TFL(LOG_FATAL, "server2venc_video Creating socket failed.");
		exit(EXIT_FAILURE);
	}


	struct sockaddr_in server2ldp_heart; /* server's address information */
	/* Creating UDP socket */
	if ((sockfd_ldp_heart = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		/* handle exception */
		perror("server2ldp_heart Creating socket failed.");
		LOG_TFL(LOG_FATAL, "server2ldp_heart Creating socket failed.");
		exit(EXIT_FAILURE);
	}
	bzero(&server2ldp_heart, sizeof(server2ldp_heart));
	server2ldp_heart.sin_family= AF_INET;

	server2ldp_heart.sin_port = htons(SERVER_PORT2LDP_HEART);
	server2ldp_heart.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd_ldp_heart, (struct sockaddr *)&server2ldp_heart, sizeof(struct sockaddr)) == -1) {
		/* handle exception */
		perror("server2ldp_heart Bind error.");
		LOG_TFL(LOG_FATAL, "server2ldp_heart Creating socket failed.");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server2ldp_video; /* server's address information */
	/* Creating UDP socket */
	if ((sockfd_ldp_video = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		/* handle exception */
		perror("sockfd_ldp_video Creating socket failed.");
		LOG_TFL(LOG_FATAL, "sockfd_ldp_video Creating socket failed.");
		exit(EXIT_FAILURE);
	}
	bzero(&server2ldp_video, sizeof(server2ldp_video));
	server2ldp_video.sin_family= AF_INET;

	server2ldp_video.sin_port = htons(SERVER_PORT2LDP_VIDEO);
	server2ldp_video.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd_ldp_video, (struct sockaddr *)&server2ldp_video, sizeof(struct sockaddr)) == -1) {
		/* handle exception */
		perror("sockfd_ldp_video Bind error.");
		LOG_TFL(LOG_FATAL, "sockfd_ldp_video Creating socket failed.");
		exit(EXIT_FAILURE);
	}

}
/*
 * 转发实时视频数据的线程
 */
void reSendVideo()
{
	byte recvmsg[LENGTH_VIDEO_DATA_BUFFER] = { 0 };
	byte video_data[LENGTH_VIDEO_DATA_BUFFER + 2] = { 0 };
	int video_data_len = 0;
	int video_data_send = 0;
	video_data[0] = 0xAA;
	video_data[1] = 0xAA;
	video_data[4] = 0x58;//flag
	video_data[5] = 0x01;//报文类型：0x01 实时视频 0x02


	struct sockaddr_in client; /* client's address information */
	socklen_t sin_size;
	int num = 0;
	sin_size = sizeof(struct sockaddr_in);
	while(true) {
		// 接收请求
		//char c = 0xFF, d = 0xFF;
		//char a = 0xFF, b = 0xFF;
		//int num_tmp = (a<<8) + b;
		//LOG_TFL(LOG_INFO, "reSendVideo before recvfrom %d = %d, %d \n", num_tmp, c, d);
		num = recvfrom(sockfd_venc_video, recvmsg, sizeof(recvmsg), 0, (struct sockaddr *)&client, &sin_size);
		if(num > sizeof(video_data) - 6)
		{
			LOG_TFL(LOG_ERROR, "reSendVideo video data is too big\n");
			continue;
		}

		video_data_len = (recvmsg[4]<<8) + recvmsg[3];
		video_data_send = (recvmsg[4]<<8) + recvmsg[3] + 8;
		LOG_TFL(LOG_DEBUG, "reSendVideo recvfrom  num = %d,video_data_len = %d [%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx]\n", num, video_data_len, recvmsg[0], recvmsg[1], recvmsg[2],
				recvmsg[3], recvmsg[4], recvmsg[5], recvmsg[6], recvmsg[7]);
		video_data[2] = video_data_send;//报文长度
		video_data[3] = video_data_send>>8;//报文长度
		video_data[6] = recvmsg[2];//通道号【01-16】08、16通道为音频通道不使用
		memcpy(video_data + 7, recvmsg + 5, video_data_len);
		video_data[video_data_send - 1] = 0x00;
		for(num = 0; num < video_data_send - 1; num++)
		{
			video_data[video_data_send - 1] += video_data[num];
		}
		//TODO LDP的数据接收端口， 在协议中没有明确规定
		//DATETIME数据结构体， 不太明确
		num = sendto(sockfd_ldp_video, (char*)video_data, video_data_send, 0, (struct sockaddr *)&_6A_LDP_Addr, sizeof(struct sockaddr_in));
		LOG_TFL(LOG_DEBUG, "reSendVideo sendto[%s:%d]video data video_data_send =%d sendto data is %d\n",
				inet_ntoa(_6A_LDP_Addr.sin_addr), ntohs(_6A_LDP_Addr.sin_port), video_data_send, num);
	}
}
//TODO 478加 //break break 报错
//set-clip-time: 1,pathname=/recdata/6A-VIDEO-HXD3C0001-西安西隆/2015-09-25/HXD3C0001_西安西隆_01_一端路况_20150925_094049.mp4,start
//set_clip_time: 4,find startpoint fail,startsec=1,-2

int main(int argc, char *argv[]) {
	Log_init("log_pqtelcmd", 2, 1024*1024*3, LOG_INFO, LOG_WITHOUT_BUFFER);
	fprintf(stderr, "%s start...\n\n", argv[0]);
	LOG_TFL(LOG_INFO, "%s start...\n\n", argv[0]);
	initSet();
	pthread_create(&recv_tid, NULL, recv_6ACentral, NULL);
	pthread_create(&heart_tid, NULL, heart_6ACentral, NULL);
	pthread_create(&vod_tid, NULL, videoOnDemand, NULL);
	pthread_create(&vod_heart_tid, NULL, videoOnDemandHeart, NULL);
	//pthread_create(&re_tid, NULL, reSendFile, NULL);
	reSendVideo();
	//一分钟后停止
	return EXIT_SUCCESS;
}
