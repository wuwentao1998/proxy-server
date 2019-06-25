#!/bin/bash

CUR_DIR=$(cd $(dirname $0); pwd)

nohup python3 -m http.server 8080 &

cd ${CUR_DIR}/../src
make
make clean
./proxy 8000&

curl "127.0.0.1:8080" > a.out
curl -x 127.0.0.1:8000 "127.0.0.1:8080" > b.out
diff a.out b.out > c.out

if [ -s c.out ]
then
    echo "=====mock test failed====="
    echo "content in diff file:"
    cat c.out
else
    echo "=====mock test done====="
fi

rm -f proxy a.out b.out c.out
cd ${CUR_DIR}
rm -f nohup.out
ps aux | grep "python3 -m http.server 8080" | grep -v "grep" | awk '{print $2}' | xargs kill -9
ps aux | grep "./proxy 8000" | grep -v "grep" | awk '{print $2}' | xargs kill -9
