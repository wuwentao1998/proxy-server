#ifndef CACHE_H
#define CACHE_H

#include "macro.h"
#include <sys/time.h>
#include <ntsid.h>

typedef struct cache_block{
    char* url;
    char* data;
    int datasize;
    int64_t time;
    pthread_rwlock_t rwlock;
} cache_block;

typedef struct cache_type{
    cache_block *cacheobjs;
    int size;
} cache_type;


void init_cache();
int read_cache(char* url, int fd);
void write_cache(char* url, char* data, int len);
void free_cache();
int64_t currentTimeMillis();

#endif
