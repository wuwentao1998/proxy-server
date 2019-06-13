#!/bin/bash

CUR_DIR=$(cd $(dirname $0); pwd)

cd ${CUR_DIR}/../src
make
cp *.o ${CUR_DIR}
make clean
rm -f proxy
cd ${CUR_DIR}
rm -f proxy.o

for file in $(find . -name "*_test.c")
do
    gcc -c ${file}
    gcc *.o -o main
    chmod +x main
    ./main

    rm -f ${file%\.*}.o
done

rm -f *.o
rm -f main

echo "=====unit test done====="

./mock.sh