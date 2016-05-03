#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int wait = 1;

static void *fun1(void *arg) {
    while (wait);
    int tmp = 2;
    for (int i = 0; i < 1000; i++) {
        tmp = tmp * 2 % (tmp / 2 + 1);
    }
    return (void *) 0;
}

static void *fun2(void *arg) {
    while (wait);
    int ala = 4;
    for (int i = 5; i >= 0; i--) {
        ala = ala / (ala - 1);
    }
    return (void *) 0;
}

int main(int argc, char *argv[]) {

    pthread_t *threads = malloc(1000 * sizeof(pthread_t));
    for (int i = 0; i < 999; i++) {
        if (pthread_create(&threads[i], NULL, fun1, NULL) != 0) {
            printf("error while creating thread\n");
            exit(2);
        }
    }
    if (pthread_create(&threads[999], NULL, fun2, NULL) != 0) {
        printf("error while creating thread\n");
        exit(2);
    }
    wait = 0;
    for (int i = 0; i < 1000; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("error while waiting for thread\n");
            exit(3);
        }
    }
    return 0;
}