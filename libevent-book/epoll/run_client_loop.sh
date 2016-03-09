#!/usr/bin/bash


for i in $(seq 1 100); do
	nohup ./epoll_async_udp_cli.exe i &
done 
