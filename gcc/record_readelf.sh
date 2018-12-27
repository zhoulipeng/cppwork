gcc -O2 -frecord-gcc-switches main.c
readelf -p .GCC.command.line a.out 
# https://stackoverflow.com/questions/12112338/get-the-compiler-options-from-a-compiled-executable
# String dump of section '.GCC.command.line':
#   [     0]  a.c
#   [     4]  -mtune=generic
#   [    13]  -march=x86-64
#   [    21]  -O2
#   [    25]  -frecord-gcc-switches

