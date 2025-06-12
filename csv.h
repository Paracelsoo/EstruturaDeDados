#ifndef CSV_H
#define CSV_H

#include <stdio.h>

typedef struct {
    int instant;
    char dteday[11]; // "YYYY-MM-DD"
    int season;
    int yr;
    int mnth;
    int hr;
    int holiday;
    int weekday;
    int workingday;
    int weathersit;
    float temp;
    float atemp;
    float hum;
    float windspeed;
    int casual;
    int registered;
    int cnt;
} BikeHour;

BikeHour* ler_csv(const char* filename, int* total_lidos);
void print_geral(BikeHour* dados, int total);
void liberar_dados(BikeHour* dados);

#endif
