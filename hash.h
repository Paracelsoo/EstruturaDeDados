#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

typedef struct BikeData {
    char dteday[11];
    int season, yr, mnth, hr;
    int holiday, weekday, workingday, weathersit;
    float atemp, hum, windspeed;
    int casual, registered, cnt;
} BikeData;


typedef struct HashNode {
    char *key;
    BikeData data;
    struct HashNode *next;
} HashNode;

typedef struct {
    int size;
    int count;
    int collisions;  // Adicione este campo
    HashNode **buckets;
} HashTable;


// Protótipos
// Correto
BikeData* buscar_hash(HashTable* table, const char* data, int hora);
bool inserir_hash(HashTable* table, const char* data, int hora, BikeData data_registro);
bool remover_hash(HashTable* table, const char* data, int hora);

HashTable *create_table(int size);

unsigned int hash_function(const char *key, int size);
void free_table(HashTable *table);
void resize_table(HashTable *table);
void criar_chave(char* buffer, size_t size, const char* data, int hora);
void print_registro(const BikeData* reg);



#endif
