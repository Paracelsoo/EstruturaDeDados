#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

typedef struct {
    char dteday[11]; // formato yyyy-mm-dd
    int season, yr, mnth, hr;
    int holiday, weekday, workingday, weathersit;
    float atemp, hum, windspeed;
    int casual, registered, cnt;
} BikeData;

typedef struct {
    char* key;
    BikeData data;
} HashItem;

typedef struct {
    HashItem* items;
    int size;
    unsigned int a, b, p;
} SubTable;

typedef struct {
    SubTable* subtables;
    int size;
    unsigned int a, b, p;
} PerfectHashTable;

PerfectHashTable* construir_tabela_perfeita(BikeData* dados, int n);
BikeData* buscar_tabela_perfeita(PerfectHashTable* tabela, const char* data, int hora);
void liberar_tabela_perfeita(PerfectHashTable* tabela);
void criar_chave(char* buffer, int bufsize, const char* data, int hora);

#endif
