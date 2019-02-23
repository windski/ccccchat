#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <pthread.h>
#include <stdbool.h>
#include "queue.h"

typedef struct thread_pool {
    int thread_nums;
    pthread_t *threads;
    bool isstop;

    queue_t *workqueue;
    pthread_mutex_t mutex;
} threadpool_t;


extern threadpool_t *threadpool_create(int _thread_num);
extern void threadpool_append(threadpool_t *pool, void *task);

extern void threadpool_destroy(threadpool_t *pool);

#endif

