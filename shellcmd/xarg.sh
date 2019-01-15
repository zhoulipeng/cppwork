# dpkg -l 命令可以浏览所有安装的包，其中 rc 状态的包即卸载了包却保留了配置文件。
# 如果想要完整删除所有 rc 状态的包一个一个删还是很麻烦的，所以可以使用以下命令进行清理
dpkg -l | grep ^rc | cut -d' ' -f3 | sudo xargs dpkg --purge  
# 查找图片，并批量重置尺寸为1024*768, 输出到 output目录
find . -name "*.png"|xargs -I {} convert -resize 1024x768 {} output/{}
