#ifndef TRIE_H
#define TRIE_H

#include "csv.h"

typedef struct TrieNode {
    char key; 
    struct TrieNode* children[128];
    BikeHour** dados; // ponteiros para registros
    int count;        // quantos registros neste nó
    int capacity;     // para realocar dados
} TrieNode;

TrieNode* trie_criar();
void trie_inserir(TrieNode* root, const char* chave, BikeHour* dado);
BikeHour** trie_buscar(TrieNode* root, const char* chave, int* count);
int trie_remover(TrieNode* root, const char* chave, BikeHour* dado);
void trie_liberar(TrieNode* root);

#endif
