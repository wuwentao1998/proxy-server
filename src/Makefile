# Makefile for Proxy

CC = gcc
CFLAGS = -std==c11 -g -Wall -O2
# LDFLAGS = -lpthread

TARGET = proxy
SOURCE = connect.o error.o http.o proxy.o robust_IO.o wrapper.o

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

connect.o: connect.c macro.h error.h
	$(CC) $(CFLAGS) -c connect.c

error.o: error.c macro.h
	$(CC) $(CFLAGS) -c error.c

http.o: http.c wrapper.h robust_IO.h
	$(CC) $(CFLAGS) -c http.c

proxy.o: proxy.c http.h connect.h
	$(CC) $(CFLAGS) -c proxy.c

robust_IO.o: robust_IO.c macro.h error.h
	$(CC) $(CFLAGS) -c robust_IO.c

wrapper.o: wrapper.c error.h
	$(CC) $(CFLAGS) -c wrapper.c


clean:
	rm -f $(SOURCE) $(TARGET)