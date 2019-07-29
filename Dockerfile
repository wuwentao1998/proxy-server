FROM gcc:latest
MAINTAINER wuwentao "winter_wu@sjtu.edu.cn"

COPY . /proxy-server
WORKDIR /proxy-server
RUN cd src/; make; make clean; cd ..; mkdir -p output/release; cp ./src/proxy output/release; cp -r log output
