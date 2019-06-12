# Makefile for Proxy

CC = gcc
CFLAGS = -g -Wall -std=c11
# LDFLAGS = -lpthread

TARGET = proxy
SOURCE = error.o http.o proxy.o robust_IO.o wrapper.o connect.o

all: $(TARGET)
	@echo proxy successfully constructed

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

connect.o: connect.c connect.h
	$(CC) $(CFLAGS) -c connect.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

http.o: http.c http.h
	$(CC) $(CFLAGS) -c http.c

proxy.o: proxy.c http.h
	$(CC) $(CFLAGS) -c proxy.c

robust_IO.o: robust_IO.c robust_IO.h
	$(CC) $(CFLAGS) -c robust_IO.c

wrapper.o: wrapper.c wrapper.h
	$(CC) $(CFLAGS) -c wrapper.c


clean:
	rm -f $(SOURCE)