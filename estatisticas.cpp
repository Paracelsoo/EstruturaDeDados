
// ==== FILE: estatisticas.cpp ====
#include "estatisticas.h"
#include <iostream>
#include <cmath>

struct Estatistica {
    int soma = 0;
    int somaQuadrado = 0;
    int total = 0;

    void adicionar(int valor) {
        soma += valor; //soma todos os valores
        somaQuadrado += valor * valor;//começo do dp
        total++;
    }

    double media() const {
        return total ? static_cast<double>(soma) / total : 0.0;//média; o : 0.0 evita divisao por zero
    }

    double desvioPadrao() const {
        if (total == 0) return 0.0;
        double m = media();
        return std::sqrt(static_cast<double>(somaQuadrado) / total - m * m);
    }
};

Estatistica horas[24];
Estatistica dias[7];
const std::string NOMES_DIAS[7] = {
    "Domingo", "Segunda-feira", "Terça-feira", 
    "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"
};
void acumular(No* raiz) {
    if (!raiz) return;//Se a árvore/subárvore for vazia (!raiz), retorna.
    acumular(raiz->esquerdo);//Percorre a subárvore esquerda primeiro (ordem crescente).

    int hr = raiz->reg.hr;
    int dia = raiz->reg.weekday;
    int valor = raiz->reg.cnt;
//Extrai dados do nó atual:
//hr: Hora do registro (0-23).
//dia: Dia da semana (0-6).
//valor: Contagem de bicicletas (cnt).
    horas[hr].adicionar(valor);
    dias[dia].adicionar(valor);

    acumular(raiz->direito);//agora percorre direita
}

void calcularEstatisticas(No* raiz) {
    acumular(raiz);//Chama acumular para preencher horas e dias.

    std::cout << "[Estatísticas por hora]\n";
    for (int i = 0; i < 24; i++) {
        std::cout << "Hora " << i << ": Média = " << horas[i].media()
                  << ", DP = " << horas[i].desvioPadrao() << "\n";
    }//Para cada hora (0-23), exibe a média e o desvio padrão de cnt.

    std::cout << "\n[Estatísticas por dia da semana]\n";
    for (int i = 0; i < 7; i++) {
        std::cout << NOMES_DIAS[i] << ": Média = " << dias[i].media()
                  << ", DP = " << dias[i].desvioPadrao() << "\n";
    }
}//Para cada dia (0-6), exibe a média e o desvio padrão.



