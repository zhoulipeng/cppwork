#!/usr/bin/bash


echo "hello world"
if [ $? -eq 0 ]
then
    echo "success"
else
    echo "failed"
fi

echo "hello world"
if [ $? -eq 0 ];
then
    echo "success"
else
    echo "failed"
fi

echo "hello world"
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "failed"
fi

echo "hello world"
if [[ $? -eq 0 ]];
then
    echo "success"
else
    echo "failed"
fi

echo "hello world"
if [[ $? -eq 0 ]];
then
    echo "success"
else
    echo "failed"
fi

echo "hello world"
if [[ $? -eq 0 ]]; then
    echo "success"
else
    echo "failed"
fi

ls /notexists

if [[ $? = 0 ]]; then
    echo "success"
else
    echo "failed"
fi

ls /notexists

if [ $? == 0 ]; then
    echo "success"
else
    echo "failed"
fi

ls /notexists

if [ "$?" = "0" ]; then
    echo "success"
else
    echo "failed"
fi

ls /notexists

if [ "$?" > "0" ]; then
    echo "success"
else
    echo "failed"
fi
