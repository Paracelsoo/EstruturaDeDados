// latencia_lista.c
#include "latencia.h"
#include <stdio.h>
#include <time.h>
#include "listaEncadeada.h"

typedef struct {
    double tempo_insert;
    double tempo_search;
    double tempo_remove;
} LatenciaLista;

static LatenciaLista latencia_lista;

void medir_latencia_lista(listaBicicletas* lista) {
    // Criando um registro de teste
    bicicletasHora registro = {
        "2011-01-01",  // dteday
        1,             // season
        0,             // yr
        1,             // mnth
        0,             // hr
        0,             // holiday
        6,             // weekday
        0,             // workingday
        1,             // weathersit
        0.2879,        // atemp
        0.81,          // hum
        0.0000,        // windspeed
        3,             // casual
        13,            // registered
        16             // cnt
    };

    // 1. Medição de inserção
    clock_t start = clock();
    inserirFinal(lista, registro);
    latencia_lista.tempo_insert = 1000.0 * (clock() - start) / CLOCKS_PER_SEC;

    // 2. Medição de busca (assumindo que existe uma função buscar_lista)
    start = clock();
    // Função de busca precisa ser implementada na listaEncadeada.h
   buscarNoFiltro(lista, registro.dteday, registro.hr);
    latencia_lista.tempo_search = 1000.0 * (clock() - start) / CLOCKS_PER_SEC;

    // 3. Medição de remoção (assumindo que existe uma função remover_lista)
    start = clock();
    // Função de remoção precisa ser implementada na listaEncadeada.h
    removerFim(lista);
    latencia_lista.tempo_remove = 1000.0 * (clock() - start) / CLOCKS_PER_SEC;
}

void mostrar_latencia_lista() {
    double media = (latencia_lista.tempo_insert + latencia_lista.tempo_search + latencia_lista.tempo_remove) / 3.0;
    
    printf("\n=== LATENCIA LISTA ENCADEADA (1 OPERACAO COMPLETA) ===\n");
    printf("Registro: 2011-01-01 hora 0\n");
    printf("Inserção: %.4f ms\n", latencia_lista.tempo_insert);
    printf("Busca:    %.4f ms\n", latencia_lista.tempo_search);
    printf("Remoção:  %.4f ms\n", latencia_lista.tempo_remove);
    printf("----------------------------\n");
    printf("Média:    %.4f ms/op\n", media);
    printf("Total:    %.4f ms\n", latencia_lista.tempo_insert + latencia_lista.tempo_search + latencia_lista.tempo_remove);
    printf("===================================================\n");
}