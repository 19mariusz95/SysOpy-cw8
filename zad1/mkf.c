#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "main.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        exit(1);
    }
    char *filename = argv[1];
    FILE *file = fopen(filename, "w");
    FILE *filer = fopen("lorem.txt", "r");
    fseek(filer, 0, SEEK_END);
    long length = ftell(filer);
    length = length - 1020;
    srand(time(NULL));
    for (int i = 0; i < 1000; i++) {
        long tmp = rand() % length;
        struct record r;
        r.id = i;
        fseek(filer, tmp, SEEK_SET);
        fread(&r.text, sizeof(char), 1020, filer);
        fwrite(&r, sizeof(r), 1, file);
    }
    fclose(file);
    fclose(filer);
}