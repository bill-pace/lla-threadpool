#include "threadpool.h"

int main() {
    threadpool_t pool;
    threadpool_init(&pool);

    threadpool_add_task(&pool, example_task, NULL);

    threadpool_destroy(&pool);

    return 0;
}
