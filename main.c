#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <psapi.h>

#include "ram.h"
#include "hash.h"
#include "csv.h"
#include "estatisticas.h"
#include "previsao.h"
#include "filtros.h"
#include "latencia.h"
#include "buscaAle.h"


// Verifica se um registro já existe na tabela
int registro_existe(HashTable* tabela, const char* data, int hora);
//Função auxiliar para exibir o menu
void exibirMenu();

int main() {
    HashTable* tabela = create_table(1000);

    if (!tabela) {
        printf("[ERRO] Falha ao criar a tabela hash.\n");
        return EXIT_FAILURE;
    }

    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar(); // Limpar buffer do teclado

        switch (opcao) {
            case 1:
                ler_csv(tabela, "hour.csv");
                printf("[SUCESSO] CSV carregado e dados inseridos na tabela hash.\n");
                break;

            case 2: {
                BikeData registro;
                char buffer_data[11];

                printf("Data (YYYY-MM-DD): ");
                scanf("%10s", buffer_data);
                printf("Hora (0 a 23): ");
                scanf("%d", &registro.hr);

                if (registro_existe(tabela, buffer_data, registro.hr)) {
                    printf("[AVISO] Registro já existe.\n");
                    continue;
                }

                strcpy(registro.dteday, buffer_data);
                printf("Season: "); scanf("%d", &registro.season);
                printf("Ano (0 ou 1): "); scanf("%d", &registro.yr);
                printf("Mês: "); scanf("%d", &registro.mnth);
                printf("Feriado (0 ou 1): "); scanf("%d", &registro.holiday);
                printf("Dia da semana (0 a 6): "); scanf("%d", &registro.weekday);
                printf("Dia útil (0 ou 1): "); scanf("%d", &registro.workingday);
                printf("Tempo (weathersit): "); scanf("%d", &registro.weathersit);
                printf("Temperatura sentida: "); scanf("%f", &registro.atemp);
                printf("Umidade: "); scanf("%f", &registro.hum);
                printf("Vento: "); scanf("%f", &registro.windspeed);
                printf("Casual: "); scanf("%d", &registro.casual);
                printf("Registrados: "); scanf("%d", &registro.registered);
                printf("Total (cnt): "); scanf("%d", &registro.cnt);

                if (inserir_hash(tabela, buffer_data, registro.hr, registro)) {
                    printf("[SUCESSO] Registro inserido.\n");
                } else {
                    printf("[ERRO] Falha na inserção do registro.\n");
                }
                break;
            }

            case 3: {
                char data[11];
                int hora;
                printf("Data (YYYY-MM-DD): "); scanf("%10s", data);
                printf("Hora: "); scanf("%d", &hora);

                BikeData* encontrado = buscar_hash(tabela, data, hora);
                if (encontrado) {
                    printf("[SUCESSO] Registro encontrado:\n");
                    print_registro(encontrado);
                } else {
                    printf("[ERRO] Registro não encontrado.\n");
                }
                break;
            }

            case 4: {
                char data[11];
                int hora;
                printf("Data (YYYY-MM-DD): "); scanf("%10s", data);
                printf("Hora: "); scanf("%d", &hora);

                if (remover_hash(tabela, data, hora)) {
                    printf("[SUCESSO] Registro removido.\n");
                } else {
                    printf("[ERRO] Registro não encontrado.\n");
                }
                break;
            }

            case 5:
                media_desvio_por_hora(tabela);
                break;

            case 6:
                media_desvio_por_semana(tabela);
                break;

            case 7: {
                size_t size;
                classificar_horarios_pico(tabela, &size);
                break;
            }

            case 8: {
                int clima;
                size_t size;
                printf("Digite o tipo de clima (1-4): ");
                scanf("%d", &clima);
                filtrar_por_clima_ordenar_demanda(tabela, clima, &size);
                break;
            }

            case 9:
                executar_previsao(tabela);
                break;

            case 10:
            medir_latencia_unica(tabela);
    		mostrar_latencia_unica();
                break;

            case 11:
			    realizar_buscas_aleatorias(tabela);
  				printf("\nTempo medio de busca: %.6f ms\n", calcular_media_buscas());
                break;
                
            case 12:{
				// Verificar uso de memória
				imprimir_estatisticas_memoria(tabela);
			    break;}
            case 0:
                printf("\n[INFO] Liberando memória...\n");
                free_table(tabela);
                printf("[INFO] Encerrando programa...\n");
                break;

            default:
                printf("[ERRO] Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return EXIT_SUCCESS;
}

int registro_existe(HashTable* tabela, const char* data, int hora) {
    BikeData* reg = buscar_hash(tabela, data, hora);
    return reg != NULL;
}

void exibirMenu() {
    printf("\n========= MENU =========\n");
    printf("1. Ler CSV\n");
    printf("2. Inserir novo registro\n");
    printf("3. Buscar registro\n");
    printf("4. Remover registro\n");
    printf("5. Estatísticas por hora\n");
    printf("6. Estatísticas por dia da semana\n");
    printf("7. Classificar horários de pico\n");
    printf("8. Filtrar por clima e ordenar por demanda\n");
    printf("9. Executar previsao\n");
    printf("10. Medir latencia\n");
    printf("11. busca aleatoria\n");
    printf("12. memoria\n");
    printf("0. Sair\n");
    printf("========================\n");
    printf("Escolha uma opção: ");
}