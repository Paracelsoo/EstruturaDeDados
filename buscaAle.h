#ifndef BUSCA_ALE_H
#define BUSCA_ALE_H

#include "listaEncadeada.h"
#include <time.h>

// Realiza 5 buscas aleatórias e retorna o tempo médio
double teste_busca_aleatoria(listaBicicletas* lista);

// Gera uma data aleatória entre 2011 e 2012
void gerar_data_aleatoria(char* data_saida);

#endif