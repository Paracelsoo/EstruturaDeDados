
// csv_loader.c
#include "csv_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_csv(const char *filename, SkipList *list) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o CSV");
        return 0;
    }

    char line[1024];
    fgets(line, sizeof(line), file); // pula cabeçalho
    int index = 0;

    while (fgets(line, sizeof(line), file)) {
        double features[MAX_FEATURES];
        int count = 0;
        char *token = strtok(line, ",");
        while (token && count < MAX_FEATURES) {
            features[count++] = atof(token);
            token = strtok(NULL, ",");
        }
        sl_insert(list, index++, features, count);
    }

    fclose(file);
    return 1;
}


