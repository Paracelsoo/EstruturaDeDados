#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "trie.h"
#include "estatisticas.h"
#include "filtros.h"

#define filename 'hour.csv'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "csv.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void imprimirDados(BikeHour** dados, int count) {
    if (dados == NULL || count == 0) {
        printf("Nenhum dado encontrado\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        printf("Registro %d:\n", i+1);
        printf("  Data: %s\n", dados[i]->dteday);
        printf("  Hora: %d\n", dados[i]->hr);
        printf("  Bicicletas: %d\n", dados[i]->cnt);
        printf("  Temperatura: %.2f\n", dados[i]->temp);
        printf("  Umidade: %.2f\n", dados[i]->hum);
        printf("----------------------------\n");
    }
}

int main() {
    TrieNode* root = trie_criar();
    int opcao;
    char chave[100];
    BikeHour* novoDado = NULL;
    BikeHour** resultados = NULL;
    int count = 0;

    do {
        printf("\n=== MENU TRIE ===\n");
        printf("1. Carregar dados do CSV (ler_csv)\n");
        printf("2. Inserir registro manualmente\n");
        printf("3. Buscar registros por chave\n");
        printf("4. Remover registro\n");
        printf("5. Visualizar registros de uma chave\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBuffer();
            continue;
        }

        switch (opcao) {
            case 1: {
                int total_registros;
                BikeHour* dados = ler_csv(filename, &total_registros);
                if (dados == NULL || total_registros == 0) {
                    printf("Falha ao carregar dados do CSV.\n");
                    break;
                }

                print_geral(dados, total_registros);

                printf("\nInserindo dados na Trie...\n");
                for (int i = 0; i < total_registros; i++) {
                    // Criar chave única combinando data e hora
                    char chave[50];
                    snprintf(chave, sizeof(chave), "%s_%02d", dados[i].dteday, dados[i].hr);
                    
                    // Alocar e copiar o registro para inserir na Trie
                    BikeHour* novo = (BikeHour*)malloc(sizeof(BikeHour));
                    if (novo) {
                        *novo = dados[i];
                        trie_inserir(root, chave, novo);
                    }
                }
                
                printf("%d registros inseridos na Trie com sucesso.\n", total_registros);
                liberar_dados(dados);
                break;
            }

            case 2: {
                novoDado = (BikeHour*)malloc(sizeof(BikeHour));
                if (!novoDado) {
                    printf("Erro ao alocar memoria!\n");
                    break;
                }
                
                printf("Digite a chave (formato: DATA_HORA): ");
                scanf("%99s", chave);
                limparBuffer();
                
                printf("Data (AAAA-MM-DD): ");
                scanf("%10s", novoDado->dteday);
                limparBuffer();
                
                printf("Hora (0-23): ");
                scanf("%d", &novoDado->hr);
                limparBuffer();
                
                printf("Total de bicicletas: ");
                scanf("%d", &novoDado->cnt);
                limparBuffer();
                
                printf("Temperatura (0-1): ");
                scanf("%f", &novoDado->temp);
                limparBuffer();
                
                trie_inserir(root, chave, novoDado);
                printf("Registro inserido com sucesso!\n");
                break;
            }

            case 3: {
                printf("Digite a chave para busca: ");
                scanf("%99s", chave);
                limparBuffer();
                
                resultados = trie_buscar(root, chave, &count);
                if (resultados && count > 0) {
                    printf("\n=== RESULTADOS (%d) ===\n", count);
                    imprimirDados(resultados, count);
                } else {
                    printf("Nenhum registro encontrado para a chave '%s'\n", chave);
                }
                break;
            }

            case 4: {
                printf("Digite a chave do registro a remover: ");
                scanf("%99s", chave);
                limparBuffer();
                
                BikeHour criterio;
                printf("Data exata (AAAA-MM-DD): ");
                scanf("%10s", criterio.dteday);
                limparBuffer();
                
                printf("Hora exata (0-23): ");
                scanf("%d", &criterio.hr);
                limparBuffer();
                
                if (trie_remover(root, chave, &criterio)) {
                    printf("Registro removido com sucesso!\n");
                } else {
                    printf("Registro não encontrado para remoção.\n");
                }
                break;
            }

            case 5: {
                printf("Digite a chave para visualização: ");
                scanf("%99s", chave);
                limparBuffer();
                
                resultados = trie_buscar(root, chave, &count);
                imprimirDados(resultados, count);
                break;
            }

            case 0: {
                printf("Liberando memoria e saindo...\n");
                trie_liberar(root);
                break;
            }

            default: {
                printf("Opcao invalida!\n");
                break;
            }
        }

    } while (opcao != 0);

    return 0;
}