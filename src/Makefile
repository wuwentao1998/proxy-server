# Makefile for Proxy

CC = gcc
CFLAGS = -std=gnu99
# LDFLAGS = -lpthread

TARGET = proxy
SOURCE = connect.o error.o http.o proxy.o robust_IO.o wrapper.o log.o sig.o

all: $(TARGET)
	@echo =====proxy successfully constructed=====

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

connect.o: connect.c connect.h macro.h error.h log.h
	$(CC) $(CFLAGS) -c connect.c

error.o: error.c error.h macro.h log.h
	$(CC) $(CFLAGS) -c error.c

http.o: http.c http.h wrapper.h robust_IO.h macro.h error.h connect.h log.h
	$(CC) $(CFLAGS) -c http.c

proxy.o: proxy.c http.h macro.h connect.h wrapper.h sig.h log.h
	$(CC) $(CFLAGS) -c proxy.c

robust_IO.o: robust_IO.c robust_IO.h macro.h wrapper.h error.h
	$(CC) $(CFLAGS) -c robust_IO.c

wrapper.o: wrapper.c wrapper.h macro.h error.h
	$(CC) $(CFLAGS) -c wrapper.c

log.o: log.c log.h macro.h
	$(CC) $(CFLAGS) -c log.c

sig.o: sig.c sig.h wrapper.h log.h
	$(CC) $(CFLAGS) -c sig.c

clean:
	-rm -f $(SOURCE)

.PHONY: all clean