#!/bin/bash
#第二种表示方法
#通过while读取行列然后读取管道循环的读取文件达到目标与效果。

find /home/zhoulp/testdir -type d -print|
sed -e  's;/home/zhoulp/testdir;/home/zhoulp/testdir1;'|
while read newdir
do
    mkdir -p  $newdir
done
