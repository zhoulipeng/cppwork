#1. 打包了包含qt动态库的绿色软件，发现动态库指向系统qt库了
#bash-5.0$ ./yxbrowser
#This application failed to start because it could not find or load the Qt platform plugin "xcb"
#in "".
#
#Reinstalling the application may fix this problem.
#已放弃
#2. 查看软件目录自带的qxcb动态库，发现指向系统qt库了
#ldd libqxcb*
#./libqxcb.so: /usr/lib64/libQt5Core.so.5: version `Qt_5.12' not found (required by ./libqxcb.so)

#3. 解决办法如下脚本所示
:<<bash
export LD_LIBRARY_PATH=/opt/maxiao:$PATH
export QT_QPA_PLATFORM_PLUGIN_PATH=/opt/maxiao/platforms
cd /opt/maxiao
./yxbrowser
bash
# 环境目录如下
:<<cat
bash-5.0$ cat /opt/maxiao/qt.conf
[Paths]
Prefix=/opt/maxiao
Plugins=platforms
cat

:<<tree
tree /opt/maxiao/

maxiao/
├── QtWebEngineProcess
├── libQt5Core.so.5
├── libQt5Core.so.5.12.8
├── libQt5DBus.so.5
├── libQt5DBus.so.5.12.8
├── libQt5Widgets.so.5
├── libQt5Widgets.so.5.12.8
├── libQt5XcbQpa.so.5
├── libQt5XcbQpa.so.5.12.8
├── libqeglfs.so
├── libqlinuxfb.so
├── libqminimal.so
├── libqminimalegl.so
├── libqoffscreen.so
├── libqvnc.so
├── libqwebgl.so
├── libqxcb.so
├── platforms
│   ├── libqeglfs.so
│   ├── libqlinuxfb.so
│   ├── libqminimal.so
│   ├── libqminimalegl.so
│   ├── libqoffscreen.so
│   ├── libqvnc.so
│   ├── libqwebgl.so
│   └── libqxcb.so
├── qt.conf
├── yxbrowser
└── yxbrowser.sh

tree

