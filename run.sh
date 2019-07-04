#!/bin/bash

VERSION=v0.2
PORT=$1
NAME=proxy

# build
docker build -t="${NAME}:${VERSION}" .
# run
docker run --net host -itd --name ${NAME} ${NAME}:${VERSION} bash -c "./${NAME} ${PORT}"

