// csv.h
#ifndef CSV_H
#define CSV_H

#include "hash.h"
#include <time.h>

// Função principal para ler o CSV e popular a tabela hash
void ler_csv(HashTable* tabela, const char* caminho);

#endif