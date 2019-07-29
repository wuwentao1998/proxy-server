FROM gcc:latest
MAINTAINER wuwentao "winter_wu@sjtu.edu.cn"

COPY . /proxy-server
WORKDIR /proxy-server
RUN cd src/; make; make clean; cd ..; mkdir output; cp ./src/proxy output; cp -r log ouput; cd output/

EXPOSE 8000