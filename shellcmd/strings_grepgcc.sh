# 查看可执行程序的编译版本信息
# 有些程序自带输出编译信息 (./ffmpeg -buildconf)
# 注意windows 与linux可执行程序都可以用类似的命名查版本信息
# 但是部分可执行程序可能查看不到
strings ./ffmpeg|grep gcc
