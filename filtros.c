#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "filtros.h"

// Função auxiliar para inserir ordenado decrescente por cnt
static void inserir_ordenado_por_cnt(listaBicicletas* lista, bicicletasHora dados) {
    No* novo = criarNo(dados);

    if (lista->inicio == NULL || dados.cnt > lista->inicio->dados.cnt) {
        novo->proximo = lista->inicio;
        lista->inicio = novo;
        if (lista->fim == NULL) lista->fim = novo;
        lista->tamanho++;
        return;
    }

    No* atual = lista->inicio;
    while (atual->proximo != NULL && atual->proximo->dados.cnt >= dados.cnt) {
        atual = atual->proximo;
    }

    novo->proximo = atual->proximo;
    atual->proximo = novo;
    if (novo->proximo == NULL) lista->fim = novo;
    lista->tamanho++;
}

// Filtra por clima e retorna nova lista (não altera original)
void filtrar_por_clima(No* inicio, int clima, listaBicicletas** saida) {
    clock_t start = clock();
    printf("Filtrando dados para clima %d\n", clima);

    *saida = (listaBicicletas*)malloc(sizeof(listaBicicletas));
    inicializarLista(*saida);

    No* atual = inicio;
    int count = 0;
    while (atual != NULL) {
        if (atual->dados.weathersit == clima) {
            inserirFinal(*saida, atual->dados);
            count++;
        }
        atual = atual->proximo;
    }

    clock_t end = clock();
    printf("Filtro concluido em %.6fs com %d registros\n", (double)(end - start) / CLOCKS_PER_SEC, count);
}

// Ordena a lista dada (ex: lista filtrada) por cnt decrescente, criando nova lista ordenada
listaBicicletas* ordenar_por_demanda_gerar_nova(listaBicicletas* lista) {
    clock_t start = clock();
    printf("Ordenando dados por demanda (cnt) decrescente\n");

    listaBicicletas* ordenada = (listaBicicletas*) malloc(sizeof(listaBicicletas));
    inicializarLista(ordenada);

    No* atual = lista->inicio;
    while (atual != NULL) {
        inserir_ordenado_por_cnt(ordenada, atual->dados);
        atual = atual->proximo;
    }

    clock_t end = clock();
    printf("Ordenacao concluida em %.6fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    return ordenada;
}
