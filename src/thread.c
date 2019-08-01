#include "thread.h"
#include <stdlib.h>
#include <string.h>

void pool_init(pool* p, int num)
{
    p->buffer = malloc(sizeof(int) * num);
    memset(p->buffer, -1, sizeof(int) * num);
    p->num = num;
    p->front = 0;
    p->rear = 0;

#ifdef MAC
    sem_unlink("mutex");
    sem_unlink("slots");
    sem_unlink("items");

    p->mutex = sem_open("mutex",O_CREAT,S_IRUSR | S_IWUSR, 1);
    p->slots = sem_open("slots", O_CREAT,S_IRUSR | S_IWUSR, num);
    p->items = sem_open("items", O_CREAT,S_IRUSR | S_IWUSR, 0);
#else
    sem_init(&p->mutex, 0, 1);
    sem_init(&p->slots,0, num);
    sem_init(&p->items, 0, 0);
#endif
}

void pool_destory(pool* p)
{
    free(p->buffer);

#ifdef MAC
    sem_close(p->slots);
    sem_close(p->items);
    sem_close(p->mutex);
#else
    sem_destroy(&p->slots);
    sem_destroy(&p->items);
    sem_destroy(&p->mutex);
#endif
}

void pool_insert(pool* p, int item)
{
#ifdef MAC
    sem_wait(p->slots);
    sem_wait(p->mutex);
#else
    sem_wait(&p->slots);
    sem_wait(&p->mutex);
#endif
    p->buffer[(++p->rear) %(p->num)] = item;
#ifdef MAC
    sem_post(p->mutex);
    sem_post(p->items);
#else
    sem_post(&p->mutex);
    sem_post(&p->items);
#endif
}

int pool_fetch(pool* p)
{
#ifdef MAC
    sem_wait(p->items);
    sem_wait(p->mutex);
#else
    sem_wait(&p->items);
    sem_wait(&p->mutex);
#endif
    int item = p->buffer[(++p->front)%(p->num)];
#ifdef MAC
    sem_post(p->mutex);
    sem_post(p->slots);
#else
    sem_post(&p->mutex);
    sem_post(&p->slots);
#endif

    return item;
}

