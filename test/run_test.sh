#!/bin/bash
CUR_DIR=$(cd $(dirname $0); pwd)
files=$(ls $CUR_DIR)

cd $CUR_DIR/../src
make
cp *.o $CUR_DIR
make clean
rm -f proxy
cd $CUR_DIR
rm -f proxy.o

for file in $files
do
    if [ $file == "run_test.sh" -o -d $file ];then
        continue
    fi

    gcc -c $file
    gcc *.o -o main
    chmod +x main
    ./main

    rm -f ${file%\.*}.o
done

rm -f *.o
rm -f main

echo "test done"
