#!/usr/bin/env bash
for std in c++98 c++03 c++0x c++11 c++14 gnu++98 gnu++11 gnu++14; do
  echo "$std :"
  g++ -std=$std -o main main.cpp
  ./main
  echo
done
echo "default :"
g++ -o main main.cpp
./main
