#ifndef CACHE_H
#define CACHE_H

#define MAX_CACHE_SIZE 100
#define MAX_URL_LENGTH 2048
#define MAX_RESPONSE_SIZE 1000000 // 1 Mo max par entrée

typedef struct {
    char url[MAX_URL_LENGTH];
    char response[MAX_RESPONSE_SIZE];
    int response_size;
} CacheEntry;

void init_cache();
int find_in_cache(const char* url, char* response, int* response_size);
void add_to_cache(const char* url, const char* response, int response_size);

#endif
