#include <iostream>
#include <vector>
#include "csv_reader.h" // Supondo que Registro está definido aqui

// Função para imprimir o tamanho do vetor de dados
void imprimirTamanhoDados(const std::vector<Registro>& dados) {
    std::cout << "Tamanho do vetor 'dados': " << dados.size() << " registros.\n";
    
    // Opcional: Mostrar o tamanho em bytes (se necessário)
    std::cout << "Tamanho total em memoria: ~" 
              << sizeof(Registro) * dados.size() 
              << " bytes (aproximadamente).\n";
}


