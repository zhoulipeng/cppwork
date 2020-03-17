# 系统随机数变量
echo "random number in [0-32767]:"
echo $RANDOM
echo "random number in [0-99]:"
echo $(($RANDOM%100))


echo "random base64 string:"
#random 随机效果较好但不能产生随机数时会阻塞
head -c 8 /dev/random|base64
#urandom 性能更好，不会阻塞
echo "random hex string:"
head -c 8 /dev/urandom |hexdump -e '1/8 "%02x"'
echo ""
head -c 4 /dev/urandom |hexdump -e '1/4 "%02x"'
# newlines
echo ""

# openssl
openssl rand -hex 10

