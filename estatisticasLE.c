#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "listaEncadeada.h"

// funcao para calcular a media de cnt por hora com temporizacao e prints organizados
float mediaCntHora(listaBicicletas* lista, int hora) {
    printf("Inicio do calculo da media cnt para hora %d\n", hora);
    clock_t inicio = clock();

    int soma = 0;
    int contador = 0;
    No* atual = lista->inicio;

    while (atual != NULL) {
        if (atual->dados.hr == hora) {
            soma += atual->dados.cnt;
            contador++;
        }
        atual = atual->proximo;
    }

    if (contador == 0) {
        printf("Nenhum dado encontrado para a hora %d\n", hora);
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Tempo gasto em mediaCntHora: %.6f segundos\n\n", tempo);
        return 0.0f;
    }

    float media = (float)soma / contador;

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Media cnt para hora %d: %.2f\n", hora, media);
    printf("Tempo gasto em mediaCntHora: %.6f segundos\n\n", tempo);
    return media;
}

// funcao para calcular a media de cnt por dia da semana com temporizacao e prints
float mediaCntDia(listaBicicletas* lista, int dia) {
    printf("Inicio do calculo da media cnt para dia %d\n", dia);
    clock_t inicio = clock();

    if (lista->inicio == NULL) {
        printf("Lista vazia.\n");
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Tempo gasto em mediaCntDia: %.6f segundos\n\n", tempo);
        return 0.0f;
    }

    int soma = 0;
    int contador = 0;
    No* atual = lista->inicio;

    while (atual != NULL) {
        if (atual->dados.weekday == dia) {
            soma += atual->dados.cnt;
            contador++;
        }
        atual = atual->proximo;
    }

    if (contador == 0) {
        printf("Nenhum dado encontrado para o dia %d.\n", dia);
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Tempo gasto em mediaCntDia: %.6f segundos\n\n", tempo);
        return 0.0f;
    }

    float media = (float)soma / contador;

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Media cnt para dia %d: %.2f\n", dia, media);
    printf("Tempo gasto em mediaCntDia: %.6f segundos\n\n", tempo);
    return media;
}

// funcao para calcular o desvio padrao por hora com temporizacao e prints
float desvioHora(listaBicicletas* lista, int hora) {
    printf("Inicio do calculo do desvio padrao para hora %d\n", hora);
    clock_t inicio = clock();

    float desvio = 0.0f, soma = 0.0f, dif = 0.0f;
    int contador = 0;
    No* atual = lista->inicio;

    float media = mediaCntHora(lista, hora);

    while (atual != NULL) {
        if (atual->dados.hr == hora) {
            dif = atual->dados.cnt - media;
            soma += dif * dif;
            contador++;
        }
        atual = atual->proximo;
    }

    if (contador == 0) {
        printf("Nenhum dado encontrado para a hora %d\n", hora);
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Tempo gasto em desvioHora: %.6f segundos\n\n", tempo);
        return 0.0f;
    }

    desvio = sqrt(soma / contador);

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Desvio padrao para hora %d: %.2f\n", hora, desvio);
    printf("Tempo gasto em desvioHora: %.6f segundos\n\n", tempo);
    return desvio;
}

// funcao para calcular o desvio padrao por dia com temporizacao e prints
float desvioCntDia(listaBicicletas* lista, int dia) {
    printf("Inicio do calculo do desvio padrao para dia %d\n", dia);
    clock_t inicio = clock();

    if (lista->inicio == NULL) {
        printf("Lista vazia.\n");
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Tempo gasto em desvioCntDia: %.6f segundos\n\n", tempo);
        return 0.0f;
    }

    float media = mediaCntDia(lista, dia);
    float soma_quadrados = 0.0f;
    int contador = 0;

    No* atual = lista->inicio;
    while (atual != NULL) {
        if (atual->dados.weekday == dia) {
            float dif = atual->dados.cnt - media;
            soma_quadrados += dif * dif;
            contador++;
        }
        atual = atual->proximo;
    }

    if (contador == 0) {
        printf("Nenhum dado encontrado para o dia %d.\n", dia);
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Tempo gasto em desvioCntDia: %.6f segundos\n\n", tempo);
        return 0.0f;
    }

    float desvio = sqrt(soma_quadrados / contador);

    const char* dias_semana[] = {
        "Domingo", "Segunda", "Terca", "Quarta",
        "Quinta", "Sexta", "Sabado"
    };

    printf("%s | Desvio padrao de cnt: %.2f\n", dias_semana[dia], desvio);

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo gasto em desvioCntDia: %.6f segundos\n\n", tempo);

    return desvio;
}
