FROM gcc:latest
MAINTAINER wuwentao "winter_wu@sjtu.edu.cn"

COPY src /proxy-server
WORKDIR /proxy-server
RUN make; make clean

EXPOSE 8000