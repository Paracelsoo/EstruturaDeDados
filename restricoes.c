#include "restricoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// R2: Implementação da verificação de limite
int verificar_limite_memoria(listaBicicletas* lista) {
    if (lista->tamanho >= MAX_ELEMENTOS) {
        printf("[R2] ERRO: Limite de memoria atingido (%d elementos)\n", MAX_ELEMENTOS);
        return 0;
    }
    return 1;
}

// R7: Simulação de CPU lenta
void simular_cpu_lenta() {
    // Gasta ciclos de CPU artificialmente
    volatile unsigned int i;
    for (i = 0; i < 1000000; i++);  // Ajuste este valor conforme necessário
}

// R13: Atraso artificial
void inserir_atraso_processamento(int milissegundos) {
    printf("[R13] Inserindo atraso de %dms...\n", milissegundos);
    usleep(milissegundos * 1000);  // Converte para microssegundos
}

// R20: Geração de dados sintéticos
void gerar_dados_sinteticos(listaBicicletas* lista, int quantidade) {
    srand(time(NULL));
    printf("[R20] Gerando %d dados sinteticos...\n", quantidade);

    for (int i = 0; i < quantidade; i++) {
        bicicletasHora registro;
        
        // Gera dados com baixa confiabilidade
        sprintf(registro.dteday, "2023-%02d-%02d", rand() % 12 + 1, rand() % 28 + 1);
        registro.season = rand() % 4 + 1;
        registro.hr = rand() % 24;
        registro.weathersit = rand() % 4 + 1;
        registro.cnt = rand() % 500;  // Valores podem ser inconsistentes
        
        // Insere com verificação de limite
        if (!verificar_limite_memoria(lista)) {
            printf("[R20] Limite de memoria atingido durante geracao\n");
            break;
        }
        
        inserirFinal(lista, registro);
    }
}