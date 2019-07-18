#!/bin/bash

echo "=====starting mock test====="

# 准备工作
CUR_DIR=$(cd $(dirname $0); pwd)

nohup python3 -m http.server 8080 &

cd ${CUR_DIR}/../src
make
make clean
./proxy 8000&

# 开始mock测试，同时10个客户端请求服务器
curl "127.0.0.1:8080" > a.out

for ((i=0; i < 10; i++))
do
    curl -x 127.0.0.1:8000 "127.0.0.1:8080" > ${i}.out
done

for ((i=0; i < 10; i++))
do
    diff a.out ${i}.out > b.out

    if [ -s b.out ]
    then
        echo "=====mock test failed====="
        echo "content in diff file:"
        cat b.out
    fi
done

# 收尾工作
echo "=====mock test done====="

rm -f proxy *.out
cd ${CUR_DIR}
rm -f nohup.out
ps aux | grep "python3 -m http.server 8080" | grep -v "grep" | awk '{print $2}' | xargs kill -9
ps aux | grep "./proxy 8000" | grep -v "grep" | awk '{print $2}' | xargs kill -9
