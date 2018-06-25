#./configure --enable-server=no   --with-elfutils=/home/hdc/zhoulp/elfutils-0.170 --prefix=/usr/local
# --without-avahi
# --without-nss
# ubuntu configure 配置变异的时候， 有很多警告，需要安装相关的库，消除这些警告
# apt install libnss3-dev libxml2-dev libvirt-dev libavahi-client-dev libavahi-common-dev
# --enable-server=no 不起作用，如果不编译 stap-serverd, use command: apt autoremove libnss3-dev
./configure --with-elfutils=/home/hdc/zhoulp/elfutils-0.170 --prefix=/usr/local
################################ stap-server + client 运行步骤
# 1, start_server
# command: /usr/local/libexec/systemtap/stap-serverd
# 然后我们会从server端的日志，启动

# sudo stap  --use-server=127.0.0.1:38417 -v -e 'probe vfs.read {printf("read performed\n"); exit()}'
# 上面这条stap语句， 会提示你 导入signing_key.x509 x509文件怎么导入呢， 还是看server端的日志更详细些
# 具体如下: 
# cd ~/.systemtap/ssl/server/moks/a1:75:eb:99:05:b5:98:d2:bf:16:4c:0b:ea:d5:44:ca:af:ba:42:4f
# sudo mokutil --import signing_key.x509
# 按照提示，reboot 系统， 继续启动server，并执行stap 发现还是不行
# 一查文件，发现要在server 主机的登录界面， 按照下列链接的步骤，完成x509文件的导入
# https://sourceware.org/systemtap/wiki/SecureBoot 
