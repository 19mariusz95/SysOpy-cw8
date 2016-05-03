#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("no enough argmuments\n");
        exit(1);
    }
    int threads;
    sscanf(argv[1], "%d", &threads);
    char *filename = argv[2];
    int records;
    sscanf(argv[3], "%d", &records);
    char *word = argv[4];

    FILE *file = fopen(filename, "r");

    return 0;
}