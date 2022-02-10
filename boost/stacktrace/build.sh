# see https://stackoverflow.com/questions/52583544/boost-stack-trace-not-showing-function-names-and-line-numbers 
#centos don't support libbacktrace , so use the next code line.
# CFLAGS=" -g -DBOOST_STACKTRACE_USE_BACKTRACE " 
CFLAGS=" -g -O2 -DBOOST_STACKTRACE_USE_ADDR2LINE "
g++ $CFLAGS -o main.o -c main.cpp 
# -lbacktrace # centos do not support libbacktrace.
g++ $CFLAGS main.o -lboost_stacktrace_basic -lboost_stacktrace_addr2line -ldl -o main.exe
rm main.o

