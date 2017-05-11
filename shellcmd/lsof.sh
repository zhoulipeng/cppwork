#统计各进程的文件描述符个数，用来确认检查 too many files open问题
lsof -n|awk '{print $1}'|sort|uniq -c |sort -nr

