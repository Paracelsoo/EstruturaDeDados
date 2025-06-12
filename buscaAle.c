#include "buscaAle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gerar_data_aleatoria(char* data_saida) {
    int ano = 2011 + rand() % 2; // 2011 ou 2012
    int mes = 1 + rand() % 12;
    int dia = 1 + rand() % 28; // Considerando máximo 28 dias para simplificar
    
    sprintf(data_saida, "%d-%02d-%02d", ano, mes, dia);
}

double teste_busca_aleatoria(listaBicicletas* lista) {
    if (lista == NULL || lista->inicio == NULL) {
        printf("Lista vazia!\n");
        return 0.0;
    }

    const int NUM_BUSCAS = 5;
    double tempo_total = 0.0;
    char data[11];
    int hora;

    printf("\n=== TESTE DE BUSCA ALEATÓRIA ===\n");
    printf("Realizando %d buscas em datas de 2011-2012...\n", NUM_BUSCAS);

    for (int i = 0; i < NUM_BUSCAS; i++) {
        // Gerar parâmetros aleatórios
        gerar_data_aleatoria(data);
        hora = rand() % 24;
        
        clock_t inicio = clock();
        No* resultado = buscarNoFiltro(lista, data, hora);
        clock_t fim = clock();
        
        double tempo_ms = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
        tempo_total += tempo_ms;
        
        printf("Busca %d: %s as %02dh - ", i+1, data, hora);
        if (resultado) {
            printf("Encontrado (%d bicicletas)", resultado->dados.cnt);
        } else {
            printf("Nao encontrado");
        }
        printf(" - %.3f ms\n", tempo_ms);
    }

    double tempo_medio = tempo_total / NUM_BUSCAS;
    printf("\nTempo medio de busca: %.3f ms\n", tempo_medio);
    printf("===============================\n");
    
    return tempo_medio;
}