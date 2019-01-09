# gnu perf 3.0.4
gperf -CGD -N IsValidCommandLineOption -K Option -L C++ -t command_options.gperf > perfecthash.hpp
g++ gperf.cpp -o gperf.exe
./gperf.exe +nolog
./gperf.exe +helpverbose
g++ command_line.cpp -o cmd.exe
./cmd.exe +nolog
./cmd.exe +helpverbose
./cmd.exe +invalid
