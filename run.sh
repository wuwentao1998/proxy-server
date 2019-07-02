#!/bin/bash

# build
docker build -t="proxy-server:v0.2"
# run
docker run -itd --name proxy -p 8000:8000 proxy-server:v0.2 bash -c "./proxy 8000&"

