#include "../inc/threadpool.h"

#include <stdlib.h>
#include <unistd.h>

int main() {
    threadpool_t pool;
    threadpool_init(&pool);

    for (int i = 0; i < QUEUE_SIZE; i++) {
        int * task_num = malloc(sizeof(int));
        *task_num = i;
        threadpool_add_task(&pool, example_task, task_num);
    }

    sleep(5);

    threadpool_destroy(&pool);

    return 0;
}
