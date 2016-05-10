#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <zconf.h>
#include <signal.h>
#include "main.h"

FILE *file;
char *word;
int records;
int threads;
pthread_t *thids;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int wait = 1;
int test_number;
int sig_number;

void sighandler(int sig) {
    printf("PID %d, TID %d, signal %d\n", getpid(), (int) pthread_self(), sig);
}

static void *thread_func(void *arg) {
    if (test_number == 4) {
        signal(sig_number, SIG_IGN);
    } else if (test_number == 3 || test_number == 5) {
        signal(sig_number, sighandler);
    }
    while (wait);
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
            sleep(1);
        }
        pthread_mutex_lock(&mutex);
        rb = fread(r, sizeof(struct record), (size_t) records, file);
        pthread_mutex_unlock(&mutex);
    }
    printf("tid: %d not found the word\n", (int) pthread_self());
    fflush(stdout);
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("no enough argmuments\n");
        exit(1);
    }
    sscanf(argv[1], "%d", &threads);
    fflush(stdout);
    char *filename = argv[2];
    sscanf(argv[3], "%d", &records);
    word = argv[4];
    test_number = atoi(argv[5]);
    sig_number = atoi(argv[6]);

    if (test_number == 2) {
        signal(sig_number, SIG_IGN);
    } else if (test_number == 3) {
        signal(sig_number, sighandler);
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("file");
        exit(-2);
    }
    thids = malloc(threads * sizeof(pthread_t));
    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thids[i], NULL, thread_func, NULL) != 0) {
            printf("error while creating new thread\n");
            exit(2);
        }
    }
    wait = 0;

    if (test_number == 1 || test_number == 2 || test_number == 3) {
        printf("send signal %d\n", sig_number);
        kill(getpid(), sig_number);
    } else if (test_number == 4 || test_number == 5) {
        printf("send signal %d to thread %d\n", sig_number, (int) thids[threads - 1]);
        pthread_kill(thids[threads - 1], sig_number);
    }

    for (int i = 0; i < threads; i++) {
        if (pthread_join(thids[i], NULL) != 0) {
            printf("error while waiting for thread\n");
            exit(3);
        }
    }

    return 0;
}