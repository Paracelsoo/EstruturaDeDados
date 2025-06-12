#include "estatisticas.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

Estatistica calcular_stats(BikeHour** dados, int count) {
    clock_t start = clock();
    printf("Calculando média e desvio padrão...\n");

    double soma = 0;
    for (int i = 0; i < count; i++) {
        soma += dados[i]->cnt;
    }
    double media = soma / count;

    double soma_quad = 0;
    for (int i = 0; i < count; i++) {
        double diff = dados[i]->cnt - media;
        soma_quad += diff * diff;
    }
    double desvio = sqrt(soma_quad / count);

    clock_t end = clock();
    printf("Cálculo concluído em %.6fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    Estatistica res = {media, desvio};
    return res;
}

void classificar_picos(BikeHour** dados, int count) {
    printf("Classificando horários de pico baseado na demanda (cnt)...\n");
    int pico_min = 50; // Exemplo de threshold fixo
    for (int i = 0; i < count; i++) {
        if (dados[i]->cnt >= pico_min) {
            printf("Horário %02d no dia %s é horário de pico (cnt=%d)\n", dados[i]->hr, dados[i]->dteday, dados[i]->cnt);
        }
    }
}
