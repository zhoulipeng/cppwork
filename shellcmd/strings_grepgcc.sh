# 查看可执行程序的版本信息，注意windows
# 与linux可执行程序都可以用类似的命名查版本信息
# 但是部分可执行程序可能查看不到
strings ./ffmpeg|grep gcc
