Nmap是一款网络扫描和主机检测的非常有用的工具。Nmap是不局限于仅仅收集信息和枚举，同时可以用来作为一个漏洞探测器或安全扫描器。它可以适用于winodws,linux,mac等操作系统

Nmap是一款非常强大的实用工具,可用于：检测活在网络上的主机（主机发现）检测主机上开放的端口（端口发现或枚举）检测到相应的端口（服务发现）的软件和版本检测操作系统，硬件地址，以及软件版本检测脆弱性的漏洞（Nmap的脚本）Nmap是一个非常普遍的工具，它有命令行界面和图形用户界面。本人包括以下方面的内容:介绍Nmap扫描中的重要参数操作系统检测Nmap使用教程Nmap使用不同的技术来执行扫描，包括：TCP的connect（）扫描，TCP反向的ident扫描，FTP反弹扫描等。所有这些扫描的类型有自己的优点和缺点，我们接下来将讨论这些问题。 Nmap的使用取决于目标主机,因为有一个简单的（基本）扫描和预先扫描之间的差异。我们需要使用一些先进的技术来绕过防火墙和入侵检测/防御系统，以获得正确的结果。下面是一些基本的命令和它们的用法的例子：扫描单一的一个主机，命令如下：
[root@localhost ~]# yum install nmap
Loaded plugins: fastestmirror
Setting up Install Process
Loading mirror speeds from cached hostfile
 * epel: ftp.cuhk.edu.hk
 * linuxtech-release: pkgrepo.linuxtech.net
 * nux-dextop: mirror.li.nux.ro
 * remi-safe: mirror.innosol.asia
 * rpmforge: mirror.oscc.org.my
 * rpmfusion-free-updates: ftp.sjtu.edu.cn
 * rpmfusion-nonfree-updates: ftp.sjtu.edu.cn
Resolving Dependencies
--> Running transaction check
---> Package nmap.x86_64 2:5.51-4.el6 will be installed
--> Finished Dependency Resolution

Dependencies Resolved

============================================================================================================================================================================================================================================
 Package                                               Arch                                                    Version                                                          Repository                                             Size
============================================================================================================================================================================================================================================
Installing:
 nmap                                                  x86_64                                                  2:5.51-4.el6                                                     base                                                  2.8 M

Transaction Summary
============================================================================================================================================================================================================================================
Install       1 Package(s)

Total download size: 2.8 M
Installed size: 9.7 M
Is this ok [y/N]: y
Downloading Packages:
nmap-5.51-4.el6.x86_64.rpm                                                                                                                                                                                           | 2.8 MB     00:01     
Running rpm_check_debug
Running Transaction Test
Transaction Test Succeeded
Running Transaction
  Installing : 2:nmap-5.51-4.el6.x86_64                                                                                                                                                                                                 1/1 
  Verifying  : 2:nmap-5.51-4.el6.x86_64                                                                                                                                                                                                 1/1 

Installed:
  nmap.x86_64 2:5.51-4.el6                                                                                                                                                                                                                  

Complete!
[root@localhost ~]# nmap -sP 192.168.100.0/24

Starting Nmap 5.51 ( http://nmap.org ) at 2016-07-23 00:05 CST
Nmap scan report for 192.168.100.1
Host is up (0.00095s latency).
MAC Address: 00:1C:10:8D:8C:BF (Cisco-Linksys)
Nmap scan report for 192.168.100.5
Host is up (0.0011s latency).
MAC Address: 00:0B:AB:F0:31:70 (Advantech Technology (china) Co.)
Nmap scan report for 192.168.100.6
Host is up (0.0012s latency).
MAC Address: 00:1D:09:0E:DC:53 (Dell)
Nmap scan report for 192.168.100.7
Host is up (0.0012s latency).
MAC Address: EC:F4:BB:CB:94:72 (Unknown)
Nmap scan report for 192.168.100.8
Host is up (0.0013s latency).
MAC Address: 94:DE:80:C9:2B:43 (Unknown)
Nmap scan report for 192.168.100.15
Host is up (0.00096s latency).
MAC Address: EC:F4:BB:CB:9B:F8 (Unknown)
Nmap scan report for 192.168.100.32
Host is up (0.0011s latency).
MAC Address: 64:00:6A:32:D4:72 (Unknown)
Nmap scan report for 192.168.100.36
Host is up (0.00056s latency).
MAC Address: 64:00:6A:32:6F:0A (Unknown)
Nmap scan report for 192.168.100.100
Host is up (0.0017s latency).
MAC Address: 6E:8E:A4:4A:F9:1C (Unknown)
Nmap scan report for 192.168.100.101
Host is up (0.0016s latency).
MAC Address: 46:EF:9B:EE:5E:32 (Unknown)
Nmap scan report for 192.168.100.102
Host is up (0.0016s latency).
MAC Address: 5A:BB:F1:8F:D5:D8 (Unknown)
Nmap scan report for 192.168.100.103
Host is up (0.0012s latency).
MAC Address: 86:05:6E:44:BF:81 (Unknown)
Nmap scan report for 192.168.100.104
Host is up (0.0012s latency).
MAC Address: 6E:6E:BC:87:16:EB (Unknown)
Nmap scan report for 192.168.100.105
Host is up (0.0013s latency).
MAC Address: F6:2C:4D:DE:C2:47 (Unknown)
Nmap scan report for 192.168.100.106
Host is up (0.0013s latency).
MAC Address: CA:F0:EC:BB:83:05 (Unknown)
Nmap scan report for 192.168.100.107
Host is up (0.0013s latency).
MAC Address: FE:8B:4D:32:BA:80 (Unknown)
Nmap scan report for 192.168.100.108
Host is up (0.00052s latency).
MAC Address: 16:5C:AC:12:E7:04 (Unknown)
Nmap scan report for 192.168.100.109
Host is up.
Nmap scan report for 192.168.100.110
Host is up (0.0036s latency).
MAC Address: D2:E4:A1:B3:E2:61 (Unknown)
Nmap scan report for 192.168.100.111
Host is up (0.0013s latency).
MAC Address: E6:5D:A5:54:A8:1F (Unknown)
Nmap scan report for 192.168.100.115
Host is up (0.00094s latency).
MAC Address: 1A:BB:65:92:F3:12 (Unknown)
Nmap scan report for 192.168.100.116
Host is up (0.0010s latency).
MAC Address: 9E:C2:07:37:B5:8E (Unknown)
Nmap scan report for 192.168.100.200
Host is up (0.00062s latency).
MAC Address: 1C:FA:68:25:5E:6D (Unknown)
Nmap scan report for 192.168.100.201
Host is up (0.0013s latency).
MAC Address: 64:00:6A:2D:61:36 (Unknown)
Nmap scan report for 192.168.100.202
Host is up (0.0013s latency).
MAC Address: 64:00:6A:2D:64:13 (Unknown)
Nmap scan report for 192.168.100.203
Host is up (0.0012s latency).
MAC Address: F4:8E:38:74:92:AC (Unknown)
Nmap scan report for 192.168.100.204
Host is up (0.0012s latency).
MAC Address: 64:00:6A:2D:61:26 (Unknown)
Nmap scan report for 192.168.100.205
Host is up (0.00034s latency).
MAC Address: 64:00:6A:2D:64:57 (Unknown)
Nmap scan report for 192.168.100.207
Host is up (0.00049s latency).
MAC Address: 64:00:6A:2D:5E:59 (Unknown)
Nmap scan report for 192.168.100.208
Host is up (0.00035s latency).
MAC Address: E6:5D:A5:54:A8:1F (Unknown)
Nmap scan report for 192.168.100.213
Host is up (0.00089s latency).
MAC Address: 64:00:6A:2D:57:88 (Unknown)
Nmap scan report for 192.168.100.214
Host is up (0.00070s latency).
MAC Address: 9E:C2:07:37:B5:8E (Unknown)
Nmap scan report for 192.168.100.216
Host is up (0.0012s latency).
MAC Address: 1A:BB:65:92:F3:12 (Unknown)
Nmap done: 256 IP addresses (33 hosts up) scanned in 2.21 seconds

