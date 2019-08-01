#ifndef THREAD_H
#define THREAD_H

#include <semaphore.h>

// #define MAC

#ifdef MAC
typedef struct
{
    int* buffer;
    int num; // max number of slots
    int front;
    int rear;
    sem_t* mutex; // protect buffer
    sem_t* slots;
    sem_t* items;
} pool;
#else
typedef struct
{
    int* buffer;
    int num; // max number of slots
    int front;
    int rear;
    sem_t mutex; // protect buffer
    sem_t slots;
    sem_t items;
} pool;
#endif

void pool_init(pool* p, int num);
void pool_destory(pool* p);
void pool_insert(pool* p, int item);
int pool_fetch(pool* p);
void* worker(void* vargp);

#endif
