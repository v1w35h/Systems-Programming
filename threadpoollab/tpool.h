#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

// Thread pool functions
void *thread_pool_init(int num_threads);
bool thread_pool_hash(void *handle, const char *directory, int hash_size);
void thread_pool_shutdown(void *handle);

// HASHING FUNCTIONS
uint32_t hash32(FILE *fl);
uint64_t hash64(FILE *fl);