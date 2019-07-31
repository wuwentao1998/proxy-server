#!/bin/bash

echo "=====starting mock test====="

# 准备工作
CUR_DIR=$(cd $(dirname $0); pwd)

nohup python3 -m http.server 8080 &

cd ${CUR_DIR}/../src
make
make clean
./proxy 8000 10 &

curl "127.0.0.1:8080" > a.out


# 开始mock测试，同时10个客户端请求服务器

# 如果是Linux系统，请使用client_linux代替client_mac
cp ../client/client_mac .
./client_mac --num=10 --proxy="127.0.0.1:8000" --url="127.0.0.1:8080"

sleep 1

for ((i=0; i < 10; i++))
do
    if [ ! -f ${i}.out ]
    then
        echo "=====mock test failed====="
        echo "file ${i}.out not exit"
        continue
    fi

    diff a.out ${i}.out > b.out

    if [ -s b.out ]
    then
        echo "=====mock test failed====="
        echo "file ${i}.out: content in diff file:"
        cat b.out
    fi
done

# 收尾工作
echo "=====mock test done====="

rm -f proxy client_mac rm -f *.out
cd ${CUR_DIR}
rm -f nohup.out
ps aux | grep "ython -m http.server 8080" | grep -v "grep" | awk '{print $2}' | xargs kill -9
ps aux | grep "proxy 8000" | grep -v "grep" | awk '{print $2}' | xargs kill -9
