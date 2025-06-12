#include <stdlib.h>
#include <string.h>
#include "filtros.h"

// Função auxiliar para comparar BikeData pelo campo cnt (decrescente)
static int cmp_cnt_desc(const void *a, const void *b) {
    const BikeData *x = (const BikeData*)a;
    const BikeData *y = (const BikeData*)b;
    return y->cnt - x->cnt;
}

BikeData* classificar_horarios_pico(HashTable* table, size_t* out_size) {
    if (!table || !out_size) return NULL;

    // Primeiro, coletar todos os registros da tabela hash em um array dinâmico
    size_t capacity = 100;
    size_t count = 0;
    BikeData* array = malloc(capacity * sizeof(BikeData));
    if (!array) return NULL;

    for (int i = 0; i < table->size; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            if (count == capacity) {
                capacity *= 2;
                BikeData *tmp = realloc(array, capacity * sizeof(BikeData));
                if (!tmp) {
                    free(array);
                    return NULL;
                }
                array = tmp;
            }
            array[count++] = node->data;
            node = node->next;
        }
    }

    // Ordenar pelo cnt decrescente
    qsort(array, count, sizeof(BikeData), cmp_cnt_desc);

    *out_size = count;
    return array;
}

BikeData* filtrar_por_clima_ordenar_demanda(HashTable* table, int clima, size_t* out_size) {
    if (!table || !out_size) return NULL;

    size_t capacity = 100;
    size_t count = 0;
    BikeData* array = malloc(capacity * sizeof(BikeData));
    if (!array) return NULL;

    for (int i = 0; i < table->size; i++) {
        HashNode *node = table->buckets[i];
        while (node) {
            if (node->data.weathersit == clima) {
                if (count == capacity) {
                    capacity *= 2;
                    BikeData *tmp = realloc(array, capacity * sizeof(BikeData));
                    if (!tmp) {
                        free(array);
                        return NULL;
                    }
                    array = tmp;
                }
                array[count++] = node->data;
            }
            node = node->next;
        }
    }

    // Ordenar pelo cnt decrescente
    qsort(array, count, sizeof(BikeData), cmp_cnt_desc);

    *out_size = count;
    return array;
}
