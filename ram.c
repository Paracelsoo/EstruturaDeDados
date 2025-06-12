#include "ram.h"
#include <stdio.h>
#include <string.h>
#include "hash.h"

size_t calcular_uso_memoria(const HashTable *table) {
    if (table == NULL) return 0;

    size_t total = 0;

    // 1. Memória da estrutura principal da tabela
    total += sizeof(HashTable);

    // 2. Memória do array de buckets (ponteiros)
    total += table->size * sizeof(HashNode*);

    // 3. Memória dos nós individuais e seus dados
    for (int i = 0; i < table->size; i++) {
        HashNode *node = table->buckets[i];
        while (node != NULL) {
            // Memória do nó em si
            total += sizeof(HashNode);
            
            // Memória da chave (string alocada dinamicamente)
            if (node->key != NULL) {
                total += strlen(node->key) + 1; // +1 para o null terminator
            }
            
            // Memória dos dados BikeData (já está incluída no sizeof(HashNode))
            // Não precisa adicionar separadamente pois é parte da struct
            
            node = node->next;
        }
    }

    return total;
}

void imprimir_estatisticas_memoria(const HashTable *table) {
    if (table == NULL) {
        printf("Tabela hash não existe.\n");
        return;
    }

    size_t total_bytes = calcular_uso_memoria(table);
    size_t bytes_por_elemento = table->count > 0 ? total_bytes / table->count : 0;

    printf("\n=== Estatísticas de Memória ===\n");
    printf("Tamanho total da tabela: %zu bytes\n", total_bytes);
    printf("Número de elementos: %d\n", table->count);
    printf("Bytes por elemento: %zu\n", bytes_por_elemento);
    printf("Número de colisões: %d\n", table->collisions);
    printf("Fator de carga: %.2f\n", (float)table->count / table->size);
    printf("==============================\n");

    // Detalhamento da memória
    printf("\nDetalhamento:\n");
    printf("- Estrutura HashTable: %zu bytes\n", sizeof(HashTable));
    printf("- Array de buckets: %zu bytes (%d buckets * %zu bytes cada)\n", 
           table->size * sizeof(HashNode*), table->size, sizeof(HashNode*));
    
    size_t nos_memoria = 0;
    size_t chaves_memoria = 0;
    int nos_count = 0;
    
    for (int i = 0; i < table->size; i++) {
        HashNode *node = table->buckets[i];
        while (node != NULL) {
            nos_count++;
            nos_memoria += sizeof(HashNode);
            if (node->key != NULL) {
                chaves_memoria += strlen(node->key) + 1;
            }
            node = node->next;
        }
    }
    
    printf("- Nós da tabela hash: %zu bytes (%d nós * %zu bytes cada)\n", 
           nos_memoria, nos_count, sizeof(HashNode));
    printf("- Chaves (strings): %zu bytes\n", chaves_memoria);
    printf("- Dados BikeData: %zu bytes (%d registros * %zu bytes cada)\n",
           nos_count * sizeof(BikeData), nos_count, sizeof(BikeData));
}