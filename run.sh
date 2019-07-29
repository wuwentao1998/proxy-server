#!/bin/bash

VERSION=v0.2
METHOD=$1
PORT=$2
NAME=proxy

if [ "${METHOD}" == "docker" ]; then

    # build
    docker build -t="${NAME}:${VERSION}" .
    # run
    docker run -p ${PORT}:${PORT} -itd --name ${NAME} ${NAME}:${VERSION} bash -c "./output/release/${NAME} ${PORT}"

elif [ "${METHOD}" == "shell" ]; then

    cd src/
    make; make clean
    cd ..
    mkdir output/release
    cp ./src/proxy output/release
    cp -r log output
    cd output/release
    ./proxy ${PORT}&

elif [ "X${METHOD}" == "X" ]; then
    echo "Lack of parameter！"
    exit 1
else
    echo "Illegal parameter！"
    exit 1
fi
