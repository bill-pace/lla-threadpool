#include "../inc/threadpool.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * thread_function(void * arg) {
    threadpool_t * pool = arg;

    while (1) {
        pthread_mutex_lock(&pool->lock);

        while (pool->queued == 0 && pool->stop == 0) {
            pthread_cond_wait(&pool->cond_var, &pool->lock);
        }

        if (pool->stop == 1) {
            pthread_mutex_unlock(&pool->lock);
            return NULL;
        }

        task_t task = pool->task_queue[pool->queue_front];
        pool->queue_front = (pool->queue_front + 1) % QUEUE_SIZE;
        pool->queued--;
        pthread_mutex_unlock(&pool->lock);

        task.fn(task.arg);
    }
}

void threadpool_init(threadpool_t * pool) {
    pool->queued = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->stop = 0;

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond_var, NULL);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(pool->threads + i, NULL, thread_function, pool);
    }
}

void threadpool_destroy(threadpool_t * pool) {
    pthread_mutex_lock(&pool->lock);
    pool->stop = 1;
    pthread_cond_broadcast(&pool->cond_var);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < THREADS; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_cond_destroy(&pool->cond_var);
    pthread_mutex_destroy(&pool->lock);
}

void threadpool_add_task(threadpool_t * pool, void (*function)(void *), void * arg) {
    pthread_mutex_lock(&pool->lock);
    if (pool->queued < QUEUE_SIZE) {
        pool->queued++;
        pool->task_queue[pool->queue_back].fn = function;
        pool->task_queue[pool->queue_back].arg = arg;
        pool->queue_back = (pool->queue_back + 1) % QUEUE_SIZE;
        pthread_cond_signal(&pool->cond_var);
    }
    pthread_mutex_unlock(&pool->lock);
}

void example_task(void * arg) {
    int * num = arg;
    printf("Processing task %d\n", *num);
    sleep(1);
    free(arg);
}
