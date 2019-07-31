#include "cache.h"
#include "log.h"
#include "robust_IO.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

cache_type caches[TYPES];
const int cache_block_size[] = {128, 1024, 5120, 10240, 25600, 102400};
const int cache_cnt[] = {40,20,20,10,10,5};

void init_cache()
{
    for (int i = 0; i < TYPES; i++)
    {
        caches[i].size = cache_cnt[i];
        caches[i].cacheobjs
                = (cache_block *)malloc(cache_cnt[i] * sizeof(cache_block));
        cache_block *j = caches[i].cacheobjs;

        for (int k = 0; k < cache_cnt[i]; j++, k++)
        {
            j->time = 0;
            j->datasize = 0;
            j->url = malloc(sizeof(char) * MAXLINE);
            strcpy(j->url,"");
            j->data = malloc(sizeof(char) * cache_block_size[i]);
            memset(j->data,0,cache_block_size[i]);
            pthread_rwlock_init(&j->rwlock,NULL);
        }
    }
}

void free_cache()
{
    for (int i = 0; i < TYPES; i++)
    {
        cache_block *j = caches[i].cacheobjs;

        for (int k = 0; k < cache_cnt[i]; j++, k++)
        {
            free(j->url);
            free(j->data);
            pthread_rwlock_destroy(&j->rwlock);
        }

        free(caches[i].cacheobjs);
    }
}

int read_cache(char *url,int fd)
{
    int i = 0;
    cache_type cur;
    cache_block *p;

    char log_string[MAXLINE];
    sprintf(log_string,"start reading cache: %s", url);
    Log(Info, "read_cache", log_string);

    for (int t = 0; t < TYPES; t++)
    {
        cur = caches[t];
        p = cur.cacheobjs;

        for(;i < cur.size; i++,p++)
        {
            if(p->time != 0 && strcmp(url,p->url) == 0)
                break;
        }

        if (i < cur.size)
            break;
    }

    if(i >= cur.size){
        Log(Info, "read_cache", "no cache");
        return -1;
    }

    pthread_rwlock_rdlock(&p->rwlock);

    if(strcmp(url,p->url) != 0){
        pthread_rwlock_unlock(&p->rwlock);
        return -1;
    }

    pthread_rwlock_unlock(&p->rwlock);

    if (!pthread_rwlock_trywrlock(&p->rwlock)) {
        p->time = currentTimeMillis();
        pthread_rwlock_unlock(&p->rwlock);
    }

    pthread_rwlock_rdlock(&p->rwlock);
    Rio_writen(fd,p->data,p->datasize);
    pthread_rwlock_unlock(&p->rwlock);

    Log(Info, "read_cache", "read cache success");
    return 0;
}

void write_cache(char *url, char *data, int len)
{
    int t = 0;
    for (; t < TYPES ; t++)
    {
        if (len <= cache_block_size[t])
            break;
    }

    char log_string[MAXLINE];
    sprintf(log_string,"start writing cache: %s %d ", url, t);
    Log(Info, "read_cache", log_string);

    /* find empty block */
    cache_type cur = caches[t];
    cache_block* p = cur.cacheobjs;

    int i = 0;
    for(;i < cur.size;i++,p++){
        if(p->time == 0){
            break;
        }
    }

    /* if not exist, find last visited */
    int64_t min = currentTimeMillis();
    cache_block* pt;
    if(i == cur.size){
        for(i=0, pt = cur.cacheobjs;i<cur.size;i++,pt++){
            if(pt->time <= min){
                min = pt->time;
                p = pt;
            }
        }
    }

    /* write the block */
    pthread_rwlock_wrlock(&p->rwlock);
    p->time = currentTimeMillis();
    p->datasize = len;
    memcpy(p->url,url,MAXLINE);
    memcpy(p->data,data,len);
    pthread_rwlock_unlock(&p->rwlock);

    Log(Info, "write_cache", "write cache success");

}

int64_t currentTimeMillis() {
    struct timeval time;
    gettimeofday(&time, NULL);
    int64_t s1 = (int64_t)(time.tv_sec) * 1000;
    int64_t s2 = (time.tv_usec / 1000);
    return s1 + s2;
}