#ifndef FILTROS_H
#define FILTROS_H

#include "listaEncadeada.h"

// Filtra por clima e retorna nova lista (não altera original)
void filtrar_por_clima(No* inicio, int clima, listaBicicletas** saida);

// Ordena a lista dada (ex: lista filtrada) por cnt decrescente, criando nova lista ordenada
listaBicicletas* ordenar_por_demanda_gerar_nova(listaBicicletas* lista);

#endif
