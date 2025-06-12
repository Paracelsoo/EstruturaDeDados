#include "ram.h"
#include "listaEncadeada.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <sys/resource.h>
#endif

double get_memory_usage() {
#ifdef _WIN32
    /* Windows */
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return (double)pmc.WorkingSetSize / (1024 * 1024); // Convertendo para MB
#else
    /* Linux e outros Unix-like */
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    return (double)r_usage.ru_maxrss / 1024; // Já está em KB em muitos sistemas
#endif
}

size_t calcular_tamanho_lista(listaBicicletas* lista) {
    if (!lista) return 0;
    
    size_t tamanho = sizeof(listaBicicletas);
    No* atual = lista->inicio;
    
    while (atual) {
        tamanho += sizeof(No) + sizeof(bicicletasHora);
        atual = atual->proximo;
    }
    
    return tamanho;
}