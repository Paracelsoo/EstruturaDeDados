#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "listaEncadeada.h"

//limpar a lista
void limparLista(listaBicicletas* lista) {
    No* atual = lista->inicio;
    No* proximo;  // declarar ponteiro auxiliar

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    lista->inicio = NULL;
    lista->fim = NULL;  // importante zerar o fim também
    lista->tamanho = 0; // zerar o tamanho da lista
}


// Função para criar um novo nó
No* criarNo(bicicletasHora valor) {
    clock_t start = clock();
    printf("\n[CRIAR NO] Iniciando criacao do no...\n");

    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("[CRIAR NO] Erro ao alocar memoria.\n");
        exit(1);
    }
    novo->dados = valor;
    novo->proximo = NULL;

    printf("[CRIAR NO] No criado com sucesso.\n");
    clock_t end = clock();
    printf("[CRIAR NO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
    return novo;
}

// Inicializacao da lista
void inicializarLista(listaBicicletas* lista) {
    clock_t start = clock();
    printf("\n[INICIALIZAR LISTA] Inicializando lista...\n");

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;

    printf("[INICIALIZAR LISTA] Lista inicializada com tamanho %d.\n", lista->tamanho);
    clock_t end = clock();
    printf("[INICIALIZAR LISTA] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Inserir no inicio da lista
void inserirInicio(listaBicicletas* lista, bicicletasHora dados) {
    clock_t start = clock();
    printf("\n[INSERIR INICIO] Antes da insercao, tamanho atual: %d\n", lista->tamanho);

    No* novo = criarNo(dados);
    if (lista->inicio == NULL) {
        lista->inicio = lista->fim = novo;
    } else {
        novo->proximo = lista->inicio;
        lista->inicio = novo;
    }
    lista->tamanho++;

    printf("[INSERIR INICIO] Insercao concluida. Novo tamanho: %d\n", lista->tamanho);
    clock_t end = clock();
    printf("[INSERIR INICIO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Inserir no final da lista
void inserirFinal(listaBicicletas* lista, bicicletasHora dados) {
    clock_t start = clock();
    printf("\n[INSERIR FINAL] Antes da insercao, tamanho atual: %d\n", lista->tamanho);

    No* novo = criarNo(dados);
    if (lista->inicio == NULL) {
        lista->inicio = lista->fim = novo;
    } else {
        lista->fim->proximo = novo;
        lista->fim = novo;
    }
    lista->tamanho++;

    printf("[INSERIR FINAL] Insercao concluida. Novo tamanho: %d\n", lista->tamanho);
    clock_t end = clock();
    printf("[INSERIR FINAL] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
}



// Busca um nó baseado no filtro (para remoção)
No* buscarNoFiltro(listaBicicletas* lista, char* date, int hr) {
    clock_t start = clock();
    printf("\n[BUSCAR NO FILTRO] Iniciando busca de no...\n");

    No* p = lista->inicio;
    while (p != NULL) {
        int condDate = (date == NULL) || (strcmp(p->dados.dteday, date) == 0);
        int condHr = (hr == -1) || (p->dados.hr == hr);

        if (condDate && condHr) {
            printf("[BUSCAR NO FILTRO] No encontrado.\n");
            clock_t end = clock();
            printf("[BUSCAR NO FILTRO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
            return p;
        }
        p = p->proximo;
    }

    printf("[BUSCAR NO FILTRO] No nao encontrado.\n");
    clock_t end = clock();
    printf("[BUSCAR NO FILTRO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
    return NULL;
}

// Remover no inicio da lista
void removerInicio(listaBicicletas* lista) {
    clock_t start = clock();
    printf("\n[REMOVER INICIO] Tamanho atual antes da remocao: %d\n", lista->tamanho);

    if (lista->inicio == NULL) {
        printf("[REMOVER INICIO] A lista ja esta vazia.\n");
        clock_t end = clock();
        printf("[REMOVER INICIO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
        return;
    }

    No* temp = lista->inicio;
    lista->inicio = lista->inicio->proximo;
    if (lista->inicio == NULL) {
        lista->fim = NULL;
    }

    free(temp);
    lista->tamanho--;

    printf("[REMOVER INICIO] Linha removida com sucesso.\n");
    printf("[REMOVER INICIO] Tamanho atual apos remocao: %d\n", lista->tamanho);

    clock_t end = clock();
    printf("[REMOVER INICIO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Remover no fim da lista
void removerFim(listaBicicletas* lista) {
    clock_t start = clock();
    printf("\n[REMOVER FIM] Tamanho atual antes da remocao: %d\n", lista->tamanho);

    if (lista->inicio == NULL) {
        printf("[REMOVER FIM] A lista ja esta vazia.\n");
        clock_t end = clock();
        printf("[REMOVER FIM] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
        return;
    }

    if (lista->inicio == lista->fim) {
        free(lista->inicio);
        lista->inicio = NULL;
        lista->fim = NULL;
    } else {
        No* atual = lista->inicio;
        while (atual->proximo != lista->fim) {
            atual = atual->proximo;
        }
        free(lista->fim);
        lista->fim = atual;
        atual->proximo = NULL;
    }

    lista->tamanho--;
    printf("[REMOVER FIM] Linha removida com sucesso.\n");
    printf("[REMOVER FIM] Tamanho atual apos remocao: %d\n", lista->tamanho);

    clock_t end = clock();
    printf("[REMOVER FIM] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Remover no meio da lista
void removerMeio(listaBicicletas* lista, char* date, int hr) {
    clock_t start = clock();
    printf("\n[REMOVER MEIO] Tamanho atual antes da remocao: %d\n", lista->tamanho);

    if (lista->inicio == NULL) {
        printf("[REMOVER MEIO] Lista vazia.\n");
        clock_t end = clock();
        printf("[REMOVER MEIO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
        return;
    }

    // Encontrar o nó a ser removido
    No* atual = buscarNoFiltro(lista, date, hr);
    if (atual == NULL) {
        printf("[REMOVER MEIO] Elemento nao encontrado para remocao.\n");
        clock_t end = clock();
        printf("[REMOVER MEIO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
        return;
    }

    // Se for o primeiro nó
    if (atual == lista->inicio) {
        lista->inicio = atual->proximo;
        if (lista->inicio == NULL) {
            lista->fim = NULL;
        }
    } else {
        // Encontrar o nó anterior
        No* anterior = lista->inicio;
        while (anterior != NULL && anterior->proximo != atual) {
            anterior = anterior->proximo;
        }

        if (anterior == NULL) {
            printf("[REMOVER MEIO] Erro interno: ponteiro anterior nulo.\n");
            clock_t end = clock();
            printf("[REMOVER MEIO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
            return;
        }

        anterior->proximo = atual->proximo;

        // Se o nó a ser removido for o último
        if (atual == lista->fim) {
            lista->fim = anterior;
        }
    }

    printf("[REMOVER MEIO] Removendo registro:\n");
    printNo(atual);

    free(atual);
    lista->tamanho--;

    printf("[REMOVER MEIO] Elemento removido com sucesso.\n");
    printf("[REMOVER MEIO] Tamanho atual apos remocao: %d\n", lista->tamanho);

    clock_t end = clock();
    printf("[REMOVER MEIO] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
}


// Imprime dados do índice i até parada
void printDadosDeAte(listaBicicletas* lista, int i, int parada) {
    clock_t start = clock();
    printf("\n[PRINT DADOS DE ATE] Imprimindo dados do indice %d ate %d\n", i, parada);

    No* atual = lista->inicio;
    int contador = 0;

    while (contador < i && atual != NULL) {
        atual = atual->proximo;
        contador++;
    }

    while (contador < parada && atual != NULL) {
        printf("Data: %s | Hr: %d | Season: %d | Yr: %d | Mes: %d | Feriado: %d | Dia util: %d\n",
               atual->dados.dteday, atual->dados.hr, atual->dados.season, atual->dados.yr, atual->dados.mnth,
               atual->dados.holiday, atual->dados.workingday);

        printf("Semana: %d | Clima: %d | Temperatura sentida: %.2f | Umidade: %.2f | Vento: %.2f\n",
               atual->dados.weekday, atual->dados.weathersit, atual->dados.atemp,
               atual->dados.hum, atual->dados.windspeed);

        printf("Casual: %d | Registrado: %d | Total: %d\n\n",
               atual->dados.casual, atual->dados.registered, atual->dados.cnt);

        atual = atual->proximo;
        contador++;
    }

    clock_t end = clock();
    printf("[PRINT DADOS DE ATE] Tempo gasto: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Imprime os dados de um nó
void printNo(No* no) {
    if (no == NULL) {
        printf("[PRINT NO] No nulo.\n");
        return;
    }
    printf("----- Registro encontrado -----\n");
    printf("Data: %s | Hr: %d | Season: %d | Yr: %d | Mes: %d | Feriado: %d | Dia util: %d\n",
        no->dados.dteday, no->dados.hr, no->dados.season, no->dados.yr, no->dados.mnth,
        no->dados.holiday, no->dados.workingday);

    printf("Semana: %d | Clima: %d | Temperatura sentida: %.2f | Umidade: %.2f | Vento: %.2f\n",
        no->dados.weekday, no->dados.weathersit, no->dados.atemp,
        no->dados.hum, no->dados.windspeed);

    printf("Casual: %d | Registrado: %d | Total: %d\n",
        no->dados.casual, no->dados.registered, no->dados.cnt);
    printf("------------------------------\n\n");
}



