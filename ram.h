#ifndef RAM_H
#define RAM_H

#include "hash.h"
#include <stddef.h>

// Função para calcular o uso total de memória da tabela hash
size_t calcular_uso_memoria(const HashTable *table);

// Função para imprimir estatísticas de memória
void imprimir_estatisticas_memoria(const HashTable *table);

#endif