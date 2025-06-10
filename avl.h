
// ==== FILE: avl.h ====
#ifndef AVL_H
#define AVL_H

#include <string>
#include <vector>

struct Registro {
    int instant;
    std::string dteday;
    int season, yr, mnth, hr, holiday, weekday, workingday, weathersit;
    float temp, atemp, hum, windspeed;
    int casual, registered, cnt;
};
struct Registros {
    int instant;
    std::string dteday;
    int season, yr, mnth, hr, holiday, weekday, workingday, weathersit;
    float temp, atemp, hum, windspeed;
    int casual, registered, cnt;
};
struct No {
    Registro reg;
    short altura;
    No* esquerdo;
    No* direito;
};

No* inserir(No* raiz, Registro reg);
No* remover(No* raiz, int instant);
No* buscar(No* raiz, int instant);
short alturaDoNo(No* no);
short fatorDeBalanceamento(No* no);
short maior(short a, short b);
No* rotacaoEsquerda(No* r);
No* rotacaoDireita(No* r);
No* balancear(No* raiz);
void emOrdem(No* raiz);
No* inserirmax(No* raiz, Registro reg);

#endif



