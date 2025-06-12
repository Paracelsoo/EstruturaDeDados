#include "latencia.h"
#include <stdio.h>
#include <time.h>

typedef struct {
    double tempo_insert;
    double tempo_search;
    double tempo_remove;
} LatenciaUnica;

static LatenciaUnica latencia;

void medir_latencia_unica(HashTable* tabela) {
    // Usando o primeiro registro do dataset
    BikeData registro = {
        0, 1, "2011-01-01", 1, 0, 1, 0, 0, 6, 0, 1, 
        0.24, 0.2879, 0.81, 0.0000, 3, 13, 16
    };

    // 1. Medição de inserção
    clock_t start = clock();
    inserir_hash(tabela, registro.dteday, registro.hr, registro);
    latencia.tempo_insert = 1000.0 * (clock() - start) / CLOCKS_PER_SEC;

    // 2. Medição de busca
    start = clock();
    buscar_hash(tabela, registro.dteday, registro.hr);
    latencia.tempo_search = 1000.0 * (clock() - start) / CLOCKS_PER_SEC;

    // 3. Medição de remoção
    start = clock();
    remover_hash(tabela, registro.dteday, registro.hr);
    latencia.tempo_remove = 1000.0 * (clock() - start) / CLOCKS_PER_SEC;
}

void mostrar_latencia_unica() {
    double media = (latencia.tempo_insert + latencia.tempo_search + latencia.tempo_remove) / 3.0;
    
    printf("\n=== LATENCIA (1 OPERACAO COMPLETA) ===\n");
    printf("Registro: 2011-01-01 hora 0\n");
    printf("Inserção: %.4f ms\n", latencia.tempo_insert);
    printf("Busca:    %.4f ms\n", latencia.tempo_search);
    printf("Remoção:  %.4f ms\n", latencia.tempo_remove);
    printf("----------------------------\n");
    printf("Média:    %.4f ms/op\n", media);
    printf("Total:    %.4f ms\n", latencia.tempo_insert + latencia.tempo_search + latencia.tempo_remove);
    printf("==============================\n");
}