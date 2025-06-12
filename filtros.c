#include "filtros.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void filtrar_por_clima(BikeHour* dados, int total, int clima, BikeHour** saida, int* saida_count) {
    clock_t start = clock();
    printf("Filtrando dados para clima %d...\n", clima);

    int c = 0;
    for (int i = 0; i < total; i++) {
        if (dados[i].weathersit == clima) {
            saida[c++] = &dados[i];
        }
    }
    *saida_count = c;

    clock_t end = clock();
    printf("Filtro concluído em %.6fs com %d registros\n", (double)(end - start) / CLOCKS_PER_SEC, c);
}

static int cmp_desc(const void* a, const void* b) {
    BikeHour* d1 = *(BikeHour**)a;
    BikeHour* d2 = *(BikeHour**)b;
    return d2->cnt - d1->cnt; // ordem decrescente
}

void ordenar_por_demanda(BikeHour** dados, int count) {
    clock_t start = clock();
    printf("Ordenando dados por demanda (cnt) decrescente...\n");

    qsort(dados, count, sizeof(BikeHour*), cmp_desc);

    clock_t end = clock();
    printf("Ordenação concluída em %.6fs\n", (double)(end - start) / CLOCKS_PER_SEC);
}
