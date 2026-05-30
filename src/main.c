#include "threadpool.h"

#include <unistd.h>

int main() {
    threadpool_t pool;
    threadpool_init(&pool);

    threadpool_add_task(&pool, example_task, NULL);

    sleep(1);

    threadpool_destroy(&pool);

    return 0;
}
