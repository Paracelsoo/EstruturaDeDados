#ifndef RESTRICOES_H
#define RESTRICOES_H

#include "listaEncadeada.h"

// R2: Restrição no tamanho máximo das estruturas de dados
#define MAX_ELEMENTOS 1000  // Limite máximo de elementos nas listas

// R7: Simulação de CPU mais lenta (1GHz)
void simular_cpu_lenta();

// R13: Inserção de atraso artificial
void inserir_atraso_processamento(int milissegundos);

// R20: Gerar dados sintéticos com baixa confiabilidade
void gerar_dados_sinteticos(listaBicicletas* lista, int quantidade);

// Função para verificar limite de memória (R2)
int verificar_limite_memoria(listaBicicletas* lista);

#endif