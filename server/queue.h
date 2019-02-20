#ifndef QUEUE_H_
#define QUEUE_H_

#include <pthread.h>
#include <stdlib.h>

struct queue_node {
    void *data;

    struct queue_node *next;
};

struct queue {
    size_t size_items;
    struct queue_node *node;

    pthread_mutex_t modify_mutex;
    pthread_mutex_t read_mutex;
};

typedef struct queue queue_t;

extern queue_t *create_queue();
extern void push_queue(queue_t *que, void *data);
extern void *pop_queue(queue_t *que);

extern void destroy_queue(queue_t *que);

#endif
