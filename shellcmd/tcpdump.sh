###################################################################################################
#                tcpdump and wireshark                                           ##################
###################################################################################################
# 最近发现用tcpdump 抓包可以解决很多问题，可以解决一些程序配合的分歧
# 也可以看到很多程序通信的细节，比如tcp connect的三次握手， tcp close的四次握手过程都可以看得清楚
# 但是tcpdump的命令经常记忆不熟， 现将tcpdump的一些用法记录， 以备查阅与练习
tcpdump host 192.168.100.146    #根据ip抓包
tcpdump ip dst 192.168.100.146  #根据目的IP抓包
