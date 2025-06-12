#ifndef MEMORIA_H
#define MEMORIA_H
#include <stddef.h>
#include "listaEncadeada.h"

// Retorna o uso de memória RAM em MB
double get_memory_usage();

// Retorna o tamanho total da lista em bytes
size_t calcular_tamanho_lista(listaBicicletas* lista);

#endif