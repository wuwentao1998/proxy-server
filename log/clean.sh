#!/bin/bash

echo "====clean expired log files===="

for file in $(find . -mtime +7)
do
    rm -f ${file}
done

