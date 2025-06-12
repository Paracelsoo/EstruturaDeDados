#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "listaEncadeada.h"

// Ler o CSV com medicao do tempo total de insercao
int lerArquivoCSV(listaBicicletas* lista) {
    FILE* arquivo = fopen("hour.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo hour.csv.\n");
        return 0;
    }

    char linha[1024]; // Buffer para cada linha do CSV
    int contador = 0;
    
    // Inicia a contagem de tempo
    clock_t inicio = clock();

    // Pular cabecalho
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        bicicletasHora registro;
        char* token = strtok(linha, ",");
        int campo = 0;

        while (token != NULL) {
            switch (campo) {
                case 0: // index (pode ignorar ou guardar)
                    break;
                case 1:  // dteday
                    strncpy(registro.dteday, token, sizeof(registro.dteday) - 1);
                    registro.dteday[sizeof(registro.dteday) - 1] = '\0';
                    break;
                case 2: registro.season = atoi(token); break;
                case 3: registro.yr = atoi(token); break;
                case 4: registro.mnth = atoi(token); break;
                case 5: registro.hr = atoi(token); break;
                case 6: registro.holiday = atoi(token); break;
                case 7: registro.weekday = atoi(token); break;
                case 8: registro.workingday = atoi(token); break;
                case 9: registro.weathersit = atoi(token); break;
                case 10: /* temp, pode ignorar */ break;
                case 11: registro.atemp = atof(token); break;
                case 12: registro.hum = atof(token); break;
                case 13: registro.windspeed = atof(token); break;
                case 14: registro.casual = atoi(token); break;
                case 15: registro.registered = atoi(token); break;
                case 16: registro.cnt = atoi(token); break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            campo++;
        }
        inserirFinal(lista, registro);
        contador++;
    }

    fclose(arquivo);
    
    // Finaliza a contagem de tempo
    clock_t fim = clock();
    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n==== Leitura e insercao do CSV ====\n");
    printf("Registros carregados: %d\n", contador);
    printf("Tempo total de insercao: %.4f segundos\n", tempo_total);
    printf("===================================\n\n");

    return 1;
}