#!/bin/bash
#grepdir.sh
#http://pslinux.blog.163.com/blog/static/86862581201101335438933/
#管道Shell生成同样的目录结构的脚本

#查找文件的目录结构、然后生成同样的文件目录与结构
find /home/zhoulp/testdir -type d -print|
#使用;作为替换的分割标记
sed -e  's;/home/zhoulp/testdir;/home/zhoulp/testdir1;'|
#开头增加了mkdir -p 参数。然后执行上面的命令。
sed -e 's/^/mkdir  -p / '|sh -x
