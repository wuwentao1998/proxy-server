#!/bin/bash

VERSION=v0.2
METHOD=$1
PORT=$2
THREAD=$3
NAME=proxy

if [ "${METHOD}" == "docker" ]; then

    # build
    docker build -t="${NAME}:${VERSION}" .
    # run
    docker run -p ${PORT}:${PORT} -itd --name ${NAME} ${NAME}:${VERSION} bash -c "./output/release/${NAME} ${PORT} ${THREAD}"

elif [ "${METHOD}" == "shell" ]; then

    cd src/
    make; make clean
    cd ..
    mkdir -p output/release
    cp ./src/${NAME} output/release
    cp -r log output
    cd output/release
    ./${NAME} ${PORT} ${THREAD} &

elif [ "X${METHOD}" == "X" ]; then
    echo "Lack of parameter！"
    exit 1
else
    echo "Illegal parameter！"
    exit 1
fi
