#include "estatisticas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void media_desvio_por_hora(HashTable* table) {
    clock_t start = clock();
    int soma[24] = {0}, cont[24] = {0};
    float media[24], desvio[24];

    for (int i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            int hora = -1;
            // Supondo que a chave contenha a hora após um underline, ex: "2021-05-01_15"
            // Ajuste o sscanf conforme seu formato real da chave
            sscanf(node->key, "%*[^_]_%d", &hora);
            if (hora >=0 && hora < 24) {
                soma[hora] += node->data.cnt;
                cont[hora]++;
            }
            node = node->next;
        }
    }

    for (int i = 0; i < 24; i++) {
        media[i] = cont[i] ? (float)soma[i] / cont[i] : 0;
    }

    for (int i = 0; i < 24; i++) {
        float sum_sq = 0;
        for (int j = 0; j < table->size; j++) {
            HashNode* node = table->buckets[j];
            while (node) {
                int hora = -1;
                sscanf(node->key, "%*[^_]_%d", &hora);
                if (hora == i) {
                    float diff = node->data.cnt - media[i];
                    sum_sq += diff * diff;
                }
                node = node->next;
            }
        }
        desvio[i] = cont[i] ? sqrt(sum_sq / cont[i]) : 0;
        printf("Hora %02d - Media: %.2f, Desvio: %.2f\n", i, media[i], desvio[i]);
    }

    clock_t end = clock();
    printf("Tempo total para media e desvio por hora: %.4fs\n", (double)(end - start) / CLOCKS_PER_SEC);
}

#include <math.h>  // para sqrt

void media_desvio_por_semana(HashTable* table) {
    clock_t start = clock();
    int soma[7] = {0}, cont[7] = {0};
    float media[7], desvio[7];

    // Primeiro, calcula soma e contagem por dia da semana
    for (int i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            int dia = node->data.weekday;  // pega direto do dado
            if (dia >= 0 && dia < 7) {
                soma[dia] += node->data.cnt;
                cont[dia]++;
            }
            node = node->next;
        }
    }

    // Calcula médias
    for (int i = 0; i < 7; i++) {
        media[i] = cont[i] ? (float)soma[i] / cont[i] : 0;
    }

    // Calcula desvio padrão
    for (int i = 0; i < 7; i++) {
        float sum_sq = 0;
        for (int j = 0; j < table->size; j++) {
            HashNode* node = table->buckets[j];
            while (node) {
                if (node->data.weekday == i) {
                    float diff = node->data.cnt - media[i];
                    sum_sq += diff * diff;
                }
                node = node->next;
            }
        }
        desvio[i] = cont[i] ? sqrt(sum_sq / cont[i]) : 0;
        printf("Dia %d - Media: %.2f, Desvio: %.2f\n", i, media[i], desvio[i]);
    }

    clock_t end = clock();
    printf("Tempo total para media e desvio por semana: %.4fs\n", (double)(end - start) / CLOCKS_PER_SEC);
}

