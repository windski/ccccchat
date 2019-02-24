#include "queue.h"
#include <assert.h>
#include <stdio.h>

queue_t *queue_create()
{
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    assert(q != NULL);

    q->size_items = 0;
    q->node = NULL;

    pthread_mutex_init(&(q->modify_mutex), NULL);
    pthread_mutex_init(&(q->read_mutex), NULL);
    pthread_mutex_lock(&(q->read_mutex));

    return q;
}


void queue_push(queue_t *que, void *data)
{
    assert(que != NULL);

    struct queue_node *node = (struct queue_node *)malloc(sizeof(struct queue_node));
    assert(node != NULL);

    node->data = data;

    pthread_mutex_lock(&(que->modify_mutex));
    
    node->next = que->node;
    que->node = node;
    que->size_items++;

    pthread_mutex_unlock(&(que->modify_mutex));
    pthread_mutex_unlock(&(que->read_mutex));
}


void *queue_pop(queue_t *que)
{
    assert(que != NULL);
    struct queue_node *cur, *pre;
    void *result;

    pthread_mutex_lock(&(que->read_mutex));
    pthread_mutex_lock(&(que->modify_mutex));

    // if is empty...
    if(!(pre = cur = que->node)) {
        pthread_mutex_unlock(&(que->modify_mutex));
        return NULL;
    }

    pre = cur = que->node;
    while(cur->next) {
        pre = cur;
        cur = cur->next;
    }

    pre->next = NULL;
    if(que->size_items == 1) {
        que->node = NULL;
    }

    que->size_items--;

    if(que->size_items > 0) {
        pthread_mutex_unlock(&(que->read_mutex));
    }

    pthread_mutex_unlock(&(que->modify_mutex));

    result = cur->data;

    free(cur);
    return result;
}


void queue_destroy(queue_t *que)
{
    assert(que != NULL);

    pthread_mutex_destroy(&(que->read_mutex));
    pthread_mutex_destroy(&(que->modify_mutex));

    if(que->size_items != 0) {
        struct queue_node *cur = que->node;

        while(cur) {
            cur = cur->next;
            free(que->node);
            que->node = cur;
        }
    }

    free(que);
}


#ifdef TEST

static void *
task(void *args)
{
    queue_t *que = (queue_t *)args;

    char src[] = "test";
    for(int i = 0; i < 100; i++) {
        queue_push(que, src);
    }

    for(int i = 0; i < 100; i++) {
        printf("%s\n", queue_pop(que));
    }

    return NULL;
}

void task1(void)
{
    printf("queue is created..\n\n");

    queue_t *que = queue_create();

    char src[] = "test";
    for(int i = 0; i < 19000; i++) {
        queue_push(que, src);
    }

    for(int i = 0; i < 10000; i++) {
        printf("%s\n ", queue_pop(que));
    }

    queue_destroy(que);

    printf("queue is destroied..\n\n");
}


int main(void)
{
#ifdef SINGLE_THREAD
    task();
#else

# define THREAD_NUMS 20
    
    queue_t *que = queue_create();

    pthread_t pids[THREAD_NUMS];
    for(int i = 0; i < THREAD_NUMS; i++) {
        pthread_create(&pids[i], NULL, task, que);
    }

    for(int i = 0; i < THREAD_NUMS; i++) {
        pthread_join(pids[i], NULL);
    }

#endif

    return 0;
}

#endif

