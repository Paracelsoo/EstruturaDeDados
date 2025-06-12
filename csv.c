#include "csv.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

BikeHour* ler_csv(const char* filename, int* total_lidos) {
    printf("Iniciando leitura do arquivo CSV: %s\n", filename);
    clock_t start = clock();

    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Erro ao abrir arquivo");
        *total_lidos = 0;
        return NULL;
    }

    // Contar linhas para alocar
    int count = 0;
    char buffer[256];
    fgets(buffer, sizeof(buffer), f); // pular header

    while (fgets(buffer, sizeof(buffer), f)) count++;

    rewind(f);
    fgets(buffer, sizeof(buffer), f); // pular header

    BikeHour* dados = malloc(sizeof(BikeHour) * count);
    if (!dados) {
        fclose(f);
        *total_lidos = 0;
        printf("Falha na alocação de memória\n");
        return NULL;
    }

    int i = 0;
    while (fgets(buffer, sizeof(buffer), f)) {
        // Parse simples pelo formato esperado
        sscanf(buffer, "%d,%10[^,],%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%d,%d,%d",
            &dados[i].instant, dados[i].dteday, &dados[i].season, &dados[i].yr,
            &dados[i].mnth, &dados[i].hr, &dados[i].holiday, &dados[i].weekday,
            &dados[i].workingday, &dados[i].weathersit, &dados[i].temp, &dados[i].atemp,
            &dados[i].hum, &dados[i].windspeed, &dados[i].casual, &dados[i].registered,
            &dados[i].cnt);
        i++;
    }
    fclose(f);

    clock_t end = clock();
    printf("Leitura finalizada: %d registros lidos em %.3fs\n", i, (double)(end - start) / CLOCKS_PER_SEC);

    *total_lidos = i;
    return dados;
}

void print_geral(BikeHour* dados, int total) {
    printf("\n--- Dados Gerais ---\n");
    printf("Total de registros lidos: %d\n", total);
    printf("Features disponíveis:\n");
    printf("instant, dteday, season, yr, mnth, hr, holiday, weekday, workingday, weathersit, temp, atemp, hum, windspeed, casual, registered, cnt\n");
}

void liberar_dados(BikeHour* dados) {
    free(dados);
}
