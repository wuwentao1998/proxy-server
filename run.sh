#!/bin/bash

VERSION=v0.2
PORT=8000
NAME=proxy

# build
docker build -t="${NAME}:${VERSION}" .
# run
docker run -itd --name ${NAME} -p ${PORT}:${PORT} ${NAME}:${VERSION} bash -c "./${NAME} ${PORT}&"

