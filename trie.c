#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

TrieNode* trie_criar() {
    TrieNode* node = calloc(1, sizeof(TrieNode));
    return node;
}

void trie_inserir(TrieNode* root, const char* chave, BikeHour* dado) {
    clock_t start = clock();
    printf("Inserindo dado na trie com chave: %s\n", chave);

    TrieNode* atual = root;
    for (int i = 0; chave[i]; i++) {
        unsigned char c = (unsigned char)chave[i];
        if (!atual->children[c]) {
            atual->children[c] = trie_criar();
        }
        atual = atual->children[c];
    }
    // Inserir dado no nó final
    if (atual->capacity == 0) {
        atual->capacity = 4;
        atual->dados = malloc(sizeof(BikeHour*) * atual->capacity);
    } else if (atual->count == atual->capacity) {
        atual->capacity *= 2;
        atual->dados = realloc(atual->dados, sizeof(BikeHour*) * atual->capacity);
    }
    atual->dados[atual->count++] = dado;

    clock_t end = clock();
    printf("Inserção concluída em %.6fs\n", (double)(end - start) / CLOCKS_PER_SEC);
}

BikeHour** trie_buscar(TrieNode* root, const char* chave, int* count) {
    clock_t start = clock();
    printf("Buscando dados na trie para chave: %s\n", chave);

    TrieNode* atual = root;
    for (int i = 0; chave[i]; i++) {
        unsigned char c = (unsigned char)chave[i];
        if (!atual->children[c]) {
            *count = 0;
            printf("Chave não encontrada\n");
            return NULL;
        }
        atual = atual->children[c];
    }
    *count = atual->count;

    clock_t end = clock();
    printf("Busca concluída em %.6fs, %d registros encontrados\n", (double)(end - start) / CLOCKS_PER_SEC, *count);

    return atual->dados;
}

int trie_remover(TrieNode* root, const char* chave, BikeHour* dado) {
    clock_t start = clock();
    printf("Removendo dado na trie para chave: %s\n", chave);

    TrieNode* atual = root;
    for (int i = 0; chave[i]; i++) {
        unsigned char c = (unsigned char)chave[i];
        if (!atual->children[c]) {
            printf("Chave não encontrada para remoção\n");
            return 0;
        }
        atual = atual->children[c];
    }

    int idx = -1;
    for (int i = 0; i < atual->count; i++) {
        if (atual->dados[i] == dado) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Dado não encontrado para remoção\n");
        return 0;
    }

    // Remover deslocando o array
    for (int i = idx; i < atual->count - 1; i++) {
        atual->dados[i] = atual->dados[i + 1];
    }
    atual->count--;

    clock_t end = clock();
    printf("Remoção concluída em %.6fs\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 1;
}

void trie_liberar(TrieNode* root) {
    if (!root) return;
    for (int i = 0; i < 128; i++) {
        if (root->children[i]) trie_liberar(root->children[i]);
    }
    if (root->dados) free(root->dados);
    free(root);
}
