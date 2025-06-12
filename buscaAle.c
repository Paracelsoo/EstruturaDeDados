#include "buscaAle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h> // Para alta precisão no Windows

#define NUM_BUSCAS 5
#define REPETICOES 10 // Número de repetições por busca

static double tempos_busca[NUM_BUSCAS];

// Função de alta precisão para Windows
static double get_high_res_time() {
    LARGE_INTEGER freq, time;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time);
    return (double)time.QuadPart / freq.QuadPart * 1000.0; // ms
}

void gerar_data_aleatoria(char* data) {
    int ano = 2011 + rand() % 2;
    int mes = 1 + rand() % 12;
    int dia = 1 + rand() % 28;
    sprintf(data, "%d-%02d-%02d", ano, mes, dia);
}

void realizar_buscas_aleatorias(HashTable* tabela) {
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < NUM_BUSCAS; i++) {
        char data[11];
        int hora = rand() % 24;
        gerar_data_aleatoria(data);
        
        // Medição com múltiplas repetições
        double inicio = get_high_res_time();
        for (int j = 0; j < REPETICOES; j++) {
            buscar_hash(tabela, data, hora);
        }
        double fim = get_high_res_time();
        
        tempos_busca[i] = (fim - inicio) / REPETICOES;
        
        printf("Busca %d: %s as %02dh - ", i+1, data, hora);
        printf("Tempo medio: %.6f ms\n", tempos_busca[i]);
    }
}

double calcular_media_buscas() {
    double soma = 0;
    for (int i = 0; i < NUM_BUSCAS; i++) {
        soma += tempos_busca[i];
    }
    return soma / NUM_BUSCAS;
}