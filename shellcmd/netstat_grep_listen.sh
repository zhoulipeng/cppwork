# 查找当前正在监听的端口
netstat -ant | grep "LISTEN"
# 查看端口被哪个进程占用
# 　 -t : 指明显示TCP端口
#　  -u : 指明显示UDP端口
#　　-l : 仅显示监听套接字(所谓套接字就是使应用程序能够读写与收发通讯协议
#         (protocol)与资料的程序)
#　　-p : 显示进程标识符和程序名称，每一个套接字/端口都属于一个程序。
#　　-n : 不进行DNS轮询，显示IP(可以加速操作)
################ tcp ########################
# 注意-p参数要有合适的权限才能看到进程名，
# 比如一般用户看不到root进程监听端口的程序名
netstat -nltp|grep 8081
############### udp ########################

#查看当前系统的连接数
netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'

