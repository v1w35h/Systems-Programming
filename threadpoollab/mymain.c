#include "tpool.h"
#include <stdio.h>

int main(void) {
    void *handle;

    handle = thread_pool_init(8);
    if (handle == NULL) {
        printf("thread_pool_init: could not initialize thread pool.\n");
        return -1;
    }
    else{
        printf("thread_pool_init worked\n");
    }
    // if (!thread_pool_hash(handle, "../threadpoollab/some", 32)) {
    //     printf("thread_pool_hash (32 bit): failed to hash /some/directory.\n");
    // }
    if (!thread_pool_hash(handle, "../threadpoollab", 32)) {
        printf("thread_pool_hash (32 bit): failed to hash /some/directory.\n");
    }
    if (!thread_pool_hash(handle, "../threadpoollab", 64)) {
        printf("thread_pool_hash (64 bit): failed to hash /another/directory.\n");
    }
    if (!thread_pool_hash(handle, "../vectorlab", 32)) {
        printf("thread_pool_hash (32 bit): failed to hash /some/directory.\n");
    }
    if (!thread_pool_hash(handle, "../vectorlab", 64)) {
        printf("thread_pool_hash (64 bit): failed to hash /another/directory.\n");
    }
        if (!thread_pool_hash(handle, "../ringbufferlab", 32)) {
        printf("thread_pool_hash (32 bit): failed to hash /some/directory.\n");
    }
    if (!thread_pool_hash(handle, "../ringbufferlab", 64)) {
        printf("thread_pool_hash (64 bit): failed to hash /another/directory.\n");
    }
    if (!thread_pool_hash(handle, "../pageallocatorlab", 32)) {
        printf("thread_pool_hash (32 bit): failed to hash /some/directory.\n");
    }
    if (!thread_pool_hash(handle, "../pageallocatorlab", 64)) {
        printf("thread_pool_hash (64 bit): failed to hash /another/directory.\n");
    }


    thread_pool_shutdown(handle);
    return 0;
}