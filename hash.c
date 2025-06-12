#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOAD_FACTOR_THRESHOLD 0.75
#define MAX_CHAVE 64

unsigned int hash_function(const char *key, int size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) hash = ((hash << 5) + hash) + c;
    return (unsigned int)(hash % size);
}

HashTable* create_table(int size) {
    clock_t start = clock();
    HashTable* table = malloc(sizeof(HashTable));
    if (!table) {
        printf("Falha ao alocar tabela hash\n");
        return NULL;
    }
    table->size = size;
    table->count = 0;
    table->collisions = 0;  // inicializa colisões
    table->buckets = calloc(size, sizeof(HashNode*));
    if (!table->buckets) {
        free(table);
        printf("Falha ao alocar buckets\n");
        return NULL;
    }
    printf("Tabela criada com tamanho %d em %.2f ms\n", size, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
    return table;
}

void free_node(HashNode* node) {
    if (!node) return;
    free_node(node->next);
    free(node->key);
    free(node);
}

void free_table(HashTable* table) {
    clock_t start = clock();
    for (int i = 0; i < table->size; i++) {
        if (table->buckets[i]) free_node(table->buckets[i]);
    }
    free(table->buckets);
    free(table);
    printf("Tabela liberada em %.2f ms\n", 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
}

// Gera a chave com base em data e hora
void criar_chave(char *dest, size_t size, const char *data, int hora) {
    snprintf(dest, size, "%s_%02d", data, hora);
}


bool inserir_hash(HashTable* table, const char* data, int hora, BikeData data_registro) {
    char chave[MAX_CHAVE];
    criar_chave(chave, MAX_CHAVE, data, hora);

    printf("Antes da insercao: %d registros\n", table->count);

    clock_t start = clock();
    unsigned int index = hash_function(chave, table->size);
    HashNode* head = table->buckets[index];

    // Verifica se a chave já existe para atualizar
    for (HashNode* curr = head; curr != NULL; curr = curr->next) {
        if (strcmp(curr->key, chave) == 0) {
            curr->data = data_registro;
            printf("Atualizado: %s em %.2f ms\n", chave, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
            printf("Depois da insercao: %d registros\n", table->count);
            return true;
        }
    }

    // Inserção de novo nó
    HashNode* new_node = malloc(sizeof(HashNode));
    if (!new_node) {
        printf("Falha ao alocar no para %s\n", chave);
        return false;
    }
    new_node->key = strdup(chave);
    if (!new_node->key) {
        free(new_node);
        printf("Falha ao duplicar chave %s\n", chave);
        return false;
    }
    new_node->data = data_registro;
    new_node->next = head;
    table->buckets[index] = new_node;

    // Se bucket já tinha nó(s), há colisão
    if (head != NULL) {
        table->collisions++;
    }

    table->count++;

    printf("Inserido: %s em %.2f ms\n", chave, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
    printf("Depois da insercao: %d registros\n", table->count);

    if ((float)table->count / table->size > LOAD_FACTOR_THRESHOLD) {
        printf("Load factor %.2f excedido. Redimensionando tabela...\n", (float)table->count / table->size);
        resize_table(table);
    }

    return true;
}

BikeData* buscar_hash(HashTable* table, const char* data, int hora) {
    char chave[MAX_CHAVE];
    criar_chave(chave, MAX_CHAVE, data, hora);

    clock_t start = clock();
    unsigned int index = hash_function(chave, table->size);
    HashNode* curr = table->buckets[index];
    while (curr != NULL) {
        if (strcmp(curr->key, chave) == 0) {
            printf("Encontrado: %s em %.2f ms\n", chave, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
            return &curr->data;
        }
        curr = curr->next;
    }
    printf("Nao encontrado: %s em %.2f ms\n", chave, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
    return NULL;
}

bool remover_hash(HashTable* table, const char* data, int hora) {
    if (!table || !data) {
        printf("Tabela ou data nula na remocao.\n");
        return false;
    }

    char chave[MAX_CHAVE];
    criar_chave(chave, MAX_CHAVE, data, hora);

    printf("Antes da remocao: %d registros\n", table->count);

    clock_t start = clock();
    unsigned int index = hash_function(chave, table->size);
    HashNode* curr = table->buckets[index];
    HashNode* prev = NULL;

    while (curr != NULL) {
        if (curr->key && strcmp(curr->key, chave) == 0) {
            if (prev) {
                prev->next = curr->next;  // Removendo nó do meio/final
            } else {
                table->buckets[index] = curr->next;  // Removendo o primeiro nó
            }

            free(curr->key);
            free(curr);
            table->count--;

            printf("Removido: %s em %.2f ms\n", chave, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
            printf("Depois da remocao: %d registros\n", table->count);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("Nao encontrado para remocao: %s em %.2f ms\n", chave, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
    printf("Depois da remocao (sem alteracao): %d registros\n", table->count);
    return false;
}


void resize_table(HashTable* table) {
    clock_t start = clock();
    int new_size = table->size * 2;
    HashNode** new_buckets = calloc(new_size, sizeof(HashNode*));
    if (!new_buckets) {
        printf("Falha ao alocar novo array para redimensionamento\n");
        return;
    }

    // Reseta contador de colisões antes do re-hash
    table->collisions = 0;

    for (int i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            HashNode* next = node->next;
            unsigned int index = hash_function(node->key, new_size);
            if (new_buckets[index] != NULL) {
                table->collisions++;
            }
            node->next = new_buckets[index];
            new_buckets[index] = node;
            node = next;
        }
    }
    free(table->buckets);
    table->buckets = new_buckets;
    table->size = new_size;
    printf("Redimensionamento concluido para tamanho %d em %.2f ms\n", new_size, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
    printf("Nova taxa de colisao: %.2f%%\n", 100.0 * table->collisions / table->count);
}

void print_registro(const BikeData* reg) {
    if (!reg) {
        printf("Registro vazio (NULL)\n");
        return;
    }
    printf("Data: %s Hora: %02d\n", reg->dteday, reg->hr);
    printf("Season: %d, Yr: %d, Mnth: %d\n", reg->season, reg->yr, reg->mnth);
    printf("Holiday: %d, Weekday: %d, Workingday: %d, Weathersit: %d\n",
           reg->holiday, reg->weekday, reg->workingday, reg->weathersit);
    printf("Atemp: %.2f, Hum: %.2f, Windspeed: %.2f\n",
           reg->atemp, reg->hum, reg->windspeed);
    printf("Casual: %d, Registered: %d, Cnt: %d\n",
           reg->casual, reg->registered, reg->cnt);
    printf("----------------------------------------\n");
}
