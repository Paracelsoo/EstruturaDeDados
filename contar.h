#ifndef CONTAR_NOS_H
#define CONTAR_NOS_H

#include <iostream>
#include <chrono>

// Função recursiva para contar o total de nós
int contarNosRecursivo(No* raiz) {
    if (!raiz) return 0;
    return 1 + contarNosRecursivo(raiz->esquerdo) + contarNosRecursivo(raiz->direito);
}

// Função principal que exibe tempo e resultado
void contarNos(No* raiz) {
    std::cout << "\n[CONTAR] Iniciando contagem de nós...\n";
    auto inicio = std::chrono::high_resolution_clock::now();

    int total = contarNosRecursivo(raiz);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "[CONTAR] Total de registros na árvore: " << total << "\n";
    std::cout << "[CONTAR] Tempo de execução: " << duracao.count() << " segundos.\n";
}

#endif // CONTAR_NOS_H


