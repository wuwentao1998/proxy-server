#!/bin/bash

echo "====clean expired log files===="

for file in $(find . -mtime +7)
do
    if [ -f ${file} ]
    then
        rm -f ${file}
    fi
done

