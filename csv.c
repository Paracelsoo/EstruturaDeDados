#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"

#define TAM_LINHA 1024
#define DELIM ","

// Função para ler o arquivo CSV e inserir os dados na tabela hash
void ler_csv(HashTable *tabela, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return;
    }

    char linha[TAM_LINHA];
    char *campos[17];
    int linhaAtual = 0;

    clock_t inicio = clock();

    // Pula o cabeçalho
    fgets(linha, TAM_LINHA, arquivo);

    while (fgets(linha, TAM_LINHA, arquivo)) {
        linhaAtual++;

        char *token = strtok(linha, DELIM);
        int i = 0;
        while (token != NULL && i < 17) {
            campos[i++] = token;
            token = strtok(NULL, DELIM);
        }

        if (i < 17) {
            printf("Linha %d com campos insuficientes.\n", linhaAtual);
            continue;
        }

        BikeData registro;

        strcpy(registro.dteday, campos[1]); // yyyy-mm-dd
        registro.season      = atoi(campos[2]);
        registro.yr          = atoi(campos[3]);
        registro.mnth        = atoi(campos[4]);
        registro.hr          = atoi(campos[5]);
        registro.holiday     = atoi(campos[6]);
        registro.weekday     = atoi(campos[7]);
        registro.workingday  = atoi(campos[8]);
        registro.weathersit  = atoi(campos[9]);
        registro.atemp       = atof(campos[11]);
        registro.hum         = atof(campos[12]);
        registro.windspeed   = atof(campos[13]);
        registro.casual      = atoi(campos[14]);
        registro.registered  = atoi(campos[15]);
        registro.cnt         = atoi(campos[16]);

        inserir_hash(tabela, registro.dteday, registro.hr, registro);
    }

    fclose(arquivo);

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo de leitura e insercao: %.3f segundos\n", tempo);
}
