# 设置网卡eth0延迟100ms 抖动20ms
tc qdisc add dev eth0 root netem delay 100ms 20ms
# 删除延迟规则
tc qdisc del dev eth0 root netem
# 设置延迟100ms 抖动20ms 60%的抖动成功率, 丢包率30% 丢包成功率　50%
tc qdisc add dev eth0 root netem delay 100ms 20ms 60% loss 30% 50%
# 查看
tc qdisc show dev eth0
