#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "main.h"

FILE *file;
char *word;
int records;
int threads;
pthread_t *thids;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func(void *arg) {
    struct record *r = malloc(records * sizeof(struct record));
    pthread_mutex_lock(&mutex);
    size_t rb = fread(r, sizeof(struct record), (size_t) records, file);
    pthread_mutex_unlock(&mutex);
    while (rb > 0) {
        for (int i = 0; i < rb; i++) {
            if (strstr(r[i].text, word) != NULL) {
                printf("found %s, tid: %d record_id:  %d\n", word, (int) pthread_self(), r->id);
                fflush(stdout);
                return (void *) 0;
            }
        }
        pthread_mutex_lock(&mutex);
        rb = fread(r, sizeof(struct record), (size_t) records, file);
        pthread_mutex_unlock(&mutex);
    }
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("no enough argmuments\n");
        exit(1);
    }
    sscanf(argv[1], "%d", &threads);
    printf("%d\n", threads);
    fflush(stdout);
    char *filename = argv[2];
    sscanf(argv[3], "%d", &records);
    word = argv[4];

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("file");
        exit(-2);
    }
    thids = malloc(threads * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thids[i], &attr, thread_func, NULL) != 0) {
            printf("error while creating new thread\n");
            exit(2);
        }
    }

    pthread_exit(0);
}