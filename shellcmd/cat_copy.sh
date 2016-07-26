#!/bin/bash
number=0
#  搜索有.c .h的文件，过滤掉含 .chm .htm .cmake文件 
find . -name "*.[ch]*" |grep -v .htm|grep -v .chm| grep -v .cmake|while read
line;do
    echo $line
    cat $line > "../out_dir/"$line
    ((number+=1))
    #echo "number1=$number" #循环内输出number值
done
echo "cat file number =$number" #管道内改变了number的值无效,依然输出0
################################################################################
#参考链接 http://sumit.blog.51cto.com/162317/271554
################################################################################
##!/bin/bash
#number=0
#while read line;do
#echo $line
#((number+=1))
#echo "number1=$number"
#done < file
#echo "number2=$number"
