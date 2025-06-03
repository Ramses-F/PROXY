#include <string.h>
#include <stdio.h>
#include "cache.h"

static CacheEntry cache[MAX_CACHE_SIZE];
static int cache_count = 0;

void init_cache() {
    cache_count = 0;
}

int find_in_cache(const char* url, char* response, int* response_size) {
    for (int i = 0; i < cache_count; i++) {
        if (strcmp(cache[i].url, url) == 0) {
            printf("[CACHE HIT] %s\n", url);
            memcpy(response, cache[i].response, cache[i].response_size);
            *response_size = cache[i].response_size;
            return 1;
        }
    }
    printf("[CACHE MISS] %s\n", url);
    return 0;
}


void add_to_cache(const char* url, const char* response, int response_size) {
    if (cache_count >= MAX_CACHE_SIZE) return;

    printf("[CACHE STORE] %s (size: %d bytes)\n", url, response_size);
    strncpy(cache[cache_count].url, url, MAX_URL_LENGTH - 1);
    memcpy(cache[cache_count].response, response, response_size);
    cache[cache_count].response_size = response_size;
    cache_count++;
}

