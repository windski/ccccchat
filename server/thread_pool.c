#include "thread_pool.h"
#include <stdlib.h>
#include <assert.h>

static void *
worker(void *args)
{
    // TODO: prepar to fill it.
}


void threadpool_append(threadpool_t *pool, void *task)
{
    assert(pool != NULL && task != NULL);

    queue_push(pool->workqueue, task);
}


threadpool_t *threadpool_create(int _thread_num)
{
    assert(_thread_num <= 0);
    threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));
    assert(pool != NULL);

    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * _thread_num);
    assert(pool->threads != NULL);

    pool->workqueue = queue_create();
    pthread_mutex_init(&(pool->mutex), NULL);

    for(int i = 0; i < _thread_num; i++) {
        // Just create the threads and set detach.
        if(pthread_create(&(pool->threads[i]), NULL, worker, pool) != 0) {
            free(pool->threads);
            free(pool);
            abort();
        }

        if(pthread_detach((pool->threads[i]))) {
            free(pool->threads);
            free(pool);
            abort();
        }
    }
    pool->isstop = false;

    return pool;
}


void threadpool_destroy(threadpool_t *pool)
{
    assert(pool != NULL);
    pool->isstop = true;

    pthread_mutex_destroy(&(pool->mutex));
    queue_destroy(pool->workqueue);
    free(pool->threads);

    free(pool);
}

