#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "listaEncadeada.h"
#include "filtros.h"
#include "estatisticasLE.h"
#include "lerCsvLE.h"
#include "restricoes.h"
#include "previsao.h"
#include "latencia.h"
#include "buscaAle.h"
#include "ram.h" // uso de memória

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void imprimirLista(listaBicicletas* lista) {
    if (lista == NULL || lista->inicio == NULL) {
        printf("Lista vazia\n");
        return;
    }
    No* atual = lista->inicio;
    while (atual) {
        printNo(atual);
        atual = atual->proximo;
    }
}

int main() {
    listaBicicletas lista;
    inicializarLista(&lista);

    listaBicicletas* listaFiltrada = NULL;
    int filtroCriado = 0;
    int opcao, clima;
    char data[25];
    int hora;

    do {
        printf("\nMenu\n");
        printf("1 - Ler CSV\n");
        printf("2 - Inserir elemento\n");
        printf("3 - Remover elemento\n");
        printf("4 - Buscar elemento\n");
        printf("5 - Imprimir lista\n");
        printf("6 - Estatisticas (media e desvio)\n");
        printf("7 - Memoria restrita e atraso de dados\n");
        printf("8 - Filtrar por clima\n");
        printf("9 - Ordenar lista por demanda\n");
        printf("10 - Ordenar lista filtrada por demanda\n");
        printf("11 - Imprimir lista original\n");
        printf("12 - Imprimir lista filtrada\n");
        printf("13 - Limpar lista filtrada\n");
        printf("14 - Prever demanda com random forest\n");
        printf("15 - CPU lenta\n");
        printf("16 - Gerar dados sinteticos\n");
        printf("17 - Tempo medio de busca aleatoria\n");
        printf("18 - Mostrar uso de memoria\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida\n");
            limparBuffer();
            continue;
        }

        switch (opcao) {
            case 1:
                limparLista(&lista);
                if (lerArquivoCSV(&lista)) {
                    printf("CSV carregado com sucesso.\n");
                } else {
                    printf("Erro ao carregar CSV\n");
                }
                break;

            case 2: {
                bicicletasHora novo;
                printf("Digite a data (AAAA-MM-DD): ");
                scanf("%s", novo.dteday);
                printf("Digite a hora (0-23): ");
                scanf("%d", &novo.hr);
                printf("Digite a temperatura ambiente (0.0-1.0): ");
                scanf("%f", &novo.atemp);
                printf("Digite a umidade (0.0-1.0): ");
                scanf("%f", &novo.hum);
                printf("Digite a velocidade do vento (0.0-1.0): ");
                scanf("%f", &novo.windspeed);
                printf("Digite o total de bicicletas: ");
                scanf("%d", &novo.cnt);
                
                inserirFinal(&lista, novo);
                printf("Elemento inserido com sucesso.\n");
                break;
            }

            case 3:
                printf("Digite a data (AAAA-MM-DD) do registro a remover: ");
                scanf("%s", data);
                printf("Digite a hora (0-23): ");
                scanf("%d", &hora);
                removerMeio(&lista, data, hora);
                printf("Remocao solicitada.\n");
                break;

            case 4: {
                printf("Digite a data (AAAA-MM-DD) do registro a buscar: ");
                scanf("%s", data);
                printf("Digite a hora (0-23): ");
                scanf("%d", &hora);
                No* encontrado = buscarNoFiltro(&lista, data, hora);
                if (encontrado) {
                    printf("Registro encontrado:\n");
                    printNo(encontrado);
                } else {
                    printf("Registro nao encontrado.\n");
                }
                break;
            }

            case 5:
                printf("Imprimindo lista original:\n");
                imprimirLista(&lista);
                break;

            case 6: {
                int subOpcao;
                printf("\nEstatisticas:\n");
                printf("1 - Media e desvio por hora do dia\n");
                printf("2 - Media e desvio por dia da semana\n");
                printf("Escolha uma opcao: ");

                if (scanf("%d", &subOpcao) != 1) {
                    printf("Entrada invalida\n");
                    limparBuffer();
                    break;
                }

                if (subOpcao == 1) {
                    printf("Digite a hora (0-23): ");
                    if (scanf("%d", &hora) != 1 || hora < 0 || hora > 23) {
                        printf("Hora invalida\n");
                        limparBuffer();
                        break;
                    }

                    float media = mediaCntHora(&lista, hora);
                    float desvio = desvioHora(&lista, hora);

                    printf("Media de bicicletas: %.2f\n", media);
                    printf("Desvio padrao: %.2f\n", desvio);
                } else if (subOpcao == 2) {
                    int dia;
                    printf("Digite o dia da semana (0=Dom): ");
                    if (scanf("%d", &dia) != 1 || dia < 0 || dia > 6) {
                        printf("Dia invalido\n");
                        limparBuffer();
                        break;
                    }

                    float media = mediaCntDia(&lista, dia);
                    float desvio = desvioCntDia(&lista, dia);

                    printf("Media de bicicletas: %.2f\n", media);
                    printf("Desvio padrao: %.2f\n", desvio);
                } else {
                    printf("Opcao invalida\n");
                }
                break;
            }

            case 7:
                limparLista(&lista);
                inserir_atraso_processamento(500);
                if (lerArquivoCSV(&lista)) {
                    printf("CSV carregado com sucesso.\n");
                    verificar_limite_memoria(&lista);
                }
                break;

            case 8:
                printf("Digite o codigo do clima (1 a 4): ");
                if (scanf("%d", &clima) != 1) {
                    printf("Entrada invalida\n");
                    limparBuffer();
                    break;
                }
                if (filtroCriado && listaFiltrada) {
                    limparLista(listaFiltrada);
                    free(listaFiltrada);
                    listaFiltrada = NULL;
                    filtroCriado = 0;
                }
                filtrar_por_clima(lista.inicio, clima, &listaFiltrada);
                filtroCriado = 1;
                printf("Filtro simples aplicado. Use opcao 12 para imprimir lista filtrada.\n");
                break;

            case 9: {
                listaBicicletas* ordenada = ordenar_por_demanda_gerar_nova(&lista);
                printf("Lista original ordenada por demanda:\n");
                imprimirLista(ordenada);
                limparLista(ordenada);
                free(ordenada);
                break;
            }

            case 10:
                if (!lista.inicio) {
                    printf("Lista vazia.\n");
                    break;
                }
                printf("Digite o codigo do clima (1 a 4): ");
                if (scanf("%d", &clima) != 1) {
                    printf("Entrada invalida\n");
                    limparBuffer();
                    break;
                }
                if (filtroCriado && listaFiltrada) {
                    limparLista(listaFiltrada);
                    free(listaFiltrada);
                }
                filtrar_por_clima(lista.inicio, clima, &listaFiltrada);
                filtroCriado = 1;
                printf("Dados filtrados:\n");
                imprimirLista(listaFiltrada);
                break;

            case 11:
                imprimirLista(&lista);
                break;

            case 12:
                if (listaFiltrada)
                    imprimirLista(listaFiltrada);
                else
                    printf("Nenhuma lista filtrada disponível.\n");
                break;

            case 13:
                if (listaFiltrada) {
                    limparLista(listaFiltrada);
                    free(listaFiltrada);
                    listaFiltrada = NULL;
                    filtroCriado = 0;
                    printf("Lista filtrada limpa.\n");
                } else {
                    printf("Nenhuma lista filtrada para limpar\n");
                }
                break;

          case 14: 
          		prever_random_forest(&lista);
          		break;



            case 15:
                medir_latencia_lista(&lista);
                mostrar_latencia_lista();
                break;

            case 16:
                gerar_dados_sinteticos(&lista, 1500);
                break;

            case 17:
                srand(time(NULL));
               teste_busca_aleatoria(&lista);  // função do buscaAle.h
                break;

            case 18:{
			
                double uso_memoria = get_memory_usage();

			    // Calcula o tamanho da lista encadeada (em bytes)
			    size_t tamanho_lista_bytes = calcular_tamanho_lista(&lista);
			
			    // Converte bytes para MB
			    double tamanho_lista_mb = (double)tamanho_lista_bytes / (1024 * 1024);
			
			    // Mostra a informação
			    printf("\n--- Informacoes de Memoria e Lista ---\n");
			    printf("Uso de memoria do processo: %.2f MB\n", uso_memoria);
			    printf("Tamanho estimado da lista encadeada: %.4f MB\n", tamanho_lista_mb);
			    printf("Quantidade de elementos na lista: %d\n", lista.tamanho);
			    printf("--------------------------------------\n");}
			    break;

            case 0:
                printf("Encerrando o programa.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    limparLista(&lista);
    if (listaFiltrada) {
        limparLista(listaFiltrada);
        free(listaFiltrada);
    }

    return 0;
}
