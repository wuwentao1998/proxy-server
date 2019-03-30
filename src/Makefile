# Makefile for Proxy

# FIXME:header file dependency problem

CC = gcc
CFLAGS = -std=c11 -g -Wall
# LDFLAGS = -lpthread

TARGET = proxy
SOURCE = connect.o error.o http.o proxy.o robust_IO.o wrapper.o

all: $(TARGET)
	@echo proxy successfully constructed

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

connect.o: connect.c macro.h error.h connect.h
	$(CC) $(CFLAGS) -c connect.c

error.o: error.c macro.h error.h
	$(CC) $(CFLAGS) -c error.c

http.o: http.c wrapper.h robust_IO.h http.h
	$(CC) $(CFLAGS) -c http.c

proxy.o: proxy.c http.h connect.h proxy.h
	$(CC) $(CFLAGS) -c proxy.c

robust_IO.o: robust_IO.c macro.h error.h robust_IO.h
	$(CC) $(CFLAGS) -c robust_IO.c

wrapper.o: wrapper.c error.h wrapper.h
	$(CC) $(CFLAGS) -c wrapper.c


clean:
	rm -f $(SOURCE) $(TARGET)