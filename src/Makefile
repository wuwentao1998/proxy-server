# Makefile for Proxy

# FIXME:header file dependency problem
# FIXME: why can;t add -std=c11 or 89 or 99...

CC = gcc
CFLAGS = -g -Wall
# LDFLAGS = -lpthread

TARGET = proxy
SOURCE = error.o http.o proxy.o robust_IO.o wrapper.o connect.o

all: $(TARGET)
	@echo proxy successfully constructed

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

connect.o: connect.h connect.c  error.h macro.h
	$(CC) $(CFLAGS) -c connect.c

error.o: error.c error.h macro.h
	$(CC) $(CFLAGS) -c error.c

http.o: http.c http.h wrapper.h robust_IO.h
	$(CC) $(CFLAGS) -c http.c

# FIXME: why can't proxy.h appears behind proxy.c
# (No rule to make target 'proxy.h', needed by 'proxy.o'.  Stop)
proxy.o: proxy.c http.h connect.h
	$(CC) $(CFLAGS) -c proxy.c

robust_IO.o: robust_IO.c robust_IO.h error.h macro.h
	$(CC) $(CFLAGS) -c robust_IO.c

wrapper.o: wrapper.c wrapper.h error.h
	$(CC) $(CFLAGS) -c wrapper.c


clean:
	rm -f $(SOURCE) $(TARGET)