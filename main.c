
// main.c
#include "skiplist.h"
#include "csv_loader.h"
#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <time.h>  // Biblioteca para medição de tempo em C
#include <psapi.h>




void print_node(SkipNode *node) {
	

    if (!node) {
        printf("Registro não encontrado.\n");
        return;
    }

    printf("Registro #%d:\n", node->index);
    if (node->feature_count < 17) {
        printf("  [Aviso] Número de features esperado: 17, mas encontrado: %d\n", node->feature_count);
    }
    printf("  1: instant = %.0f\n", node->features[0]);
    printf("  2: dteday (data, não disponível como string) = %.0f\n", node->features[1]);
    printf("  3: season = %.0f\n", node->features[2]);
    printf("  4: yr = %.0f\n", node->features[3]);
    printf("  5: mnth = %.0f\n", node->features[4]);
    printf("  6: hr = %.0f\n", node->features[5]);
    printf("  7: holiday = %.0f\n", node->features[6]);
    printf("  8: weekday = %.0f\n", node->features[7]);
    printf("  9: workingday = %.0f\n", node->features[8]);
    printf(" 10: weathersit = %.0f\n", node->features[9]);
    printf(" 11: temp = %.2f\n", node->features[10]);
    printf(" 12: atemp = %.2f\n", node->features[11]);
    printf(" 13: hum = %.2f\n", node->features[12]);
    printf(" 14: windspeed = %.2f\n", node->features[13]);
    printf(" 15: casual = %.0f\n", node->features[14]);
    printf(" 16: registered = %.0f\n", node->features[15]);
    printf(" 17: cnt = %.0f\n", node->features[16]);
    printf("\n");
}

/*teste de função, mas ela não mostra bem 
void print_node(SkipNode *node) {
    printf("Registro #%d: ", node->index);
    for (int i = 0; i < node->feature_count; i++) {
        printf("%.2f ", node->features[i]);
    }
    printf("\n");
}
*/
int main() {
    setlocale(LC_ALL, "pt_BR");
    
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Só no Windows
    #endif
	SkipList *list = sl_create(16);
	clock_t start1 = clock();
	

    // Estrutura que armazena informações de memória do processo
    PROCESS_MEMORY_COUNTERS pmc;

    // Obtém o handle do processo atual (NULL indica o processo atual)
    HANDLE hProcess = GetCurrentProcess();

    // Tenta obter as informações de uso de memória
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        // Imprime a quantidade de memória utilizada em bytes
        printf("Uso de memória (Working Set Size): %lu bytes\n", pmc.WorkingSetSize);
    } else {
        printf("Erro ao obter informações de memória.\n");
    }

    // Fecha o handle do processo (boa prática)
    CloseHandle(hProcess);

    if (!load_csv("hour.csv", list)) {
        printf("Falha ao carregar o arquivo CSV.\n");
        return 1;
    }
 	clock_t end1 = clock();
	double elapsed1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC;
 	
    printf("Dados carregados na skip list.\n");
	printf("Tempo de inserção: %.6f segundos\n\n", elapsed1);

    // Testar busca por registro
    int index = 10460;
     clock_t start2 = clock();

    SkipNode *found = sl_search_by_index(list, index);
    clock_t end2 = clock();
	double elapsed2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
 
    if (found) {
        print_node(found);
    } else {
        printf("Registro #%d não encontrado.\n\n", index);
    }
 	 printf("Tempo de busca: %.6f segundos\n\n", elapsed2);
	 printf("Teste aleatório de busca\n");
	 int indexale[] = {351, 846, 3213, 17100, 12045, 15842, 3694, 9632, 9845, 6};
	int array_size = sizeof(indexale) / sizeof(indexale[0]);  // Calculate array size correctly
   clock_t start = clock();

	for (int i = 0; i < array_size; i++) {
    SkipNode *found = sl_search_by_index(list, indexale[i]);
   
	}
	 clock_t end = clock();
	  print_node(found);
	      
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    

  printf("Tempo de busca: %.6f segundos\n", elapsed);
    

	
    // Testar inserção
    double new_data[] = {2025, 6, 7, 15, 1.0, 0.3};
    sl_insert(list, 9999, new_data, 6);
    printf("Inserido registro #9999:\n\n");
    print_node(sl_search_by_index(list, 9999));

    // Testar remoção
     clock_t start3 = clock();
    if (sl_remove(list, 100)) {
        printf("Registro #%d removido com sucesso.\n\n", 100);
    } else {
        printf("Falha ao remover registro #%d.\n\n", 100);
    }
     clock_t end3 = clock();
    double elapsed3 = ((double)(end3 - start3)) / CLOCKS_PER_SEC;
   printf("Tempo de remoção: %.6f segundos\n", elapsed3);
	double lat1 = ( elapsed3+elapsed2+elapsed1)/3;
	printf("Tempo de latência:%.6f segundos\n", lat1);
	 found = sl_search_by_index(list, 100);
    if (found) {
        printf("ERRO: Registro #%d ainda encontrado após remoção!\n", 100);
        print_node(found);
    } else {
        printf("Confirmação: Registro #%d não encontrado após remoção.\n", 100);
    }

    sl_free(list);
    
    return 0;
}


