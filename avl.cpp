// ==== FILE: avl.cpp ====
#include "avl.h"
#include <iostream>
#include <cstdlib>

short alturaDoNo(No* no) {
    return no ? no->altura : -1; // retorna -1 se o só for nulo, senão retorna a altura
}

short maior(short a, short b) {
    return (a > b) ? a : b; //operacao para retornar maior
}
//balanceamento à esquerda
No* rotacaoEsquerda(No* r) {
    No* y = r->direito;
    No* f = y->esquerdo;
    y->esquerdo = r;
    r->direito = f;
    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;
    return y;
}
//balanceamento à direita
No* rotacaoDireita(No* r) {
    No* y = r->esquerdo;
    No* f = y->direito;
    y->direito = r;
    r->esquerdo = f;
    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;
    return y;
}
//fator de balanceamento: diferença entre as alturas de subárvores esquerda-direita
short fatorDeBalanceamento(No* no) {
    return no ? alturaDoNo(no->esquerdo) - alturaDoNo(no->direito) : 0;
}
//balancea de acordo com o fator
No* balancear(No* raiz) {
    short fb = fatorDeBalanceamento(raiz);
//B. direita
    if (fb > 1) {
        if (fatorDeBalanceamento(raiz->esquerdo) < 0)
            raiz->esquerdo = rotacaoEsquerda(raiz->esquerdo);
        return rotacaoDireita(raiz);
    } else if (fb < -1) {
    	//B. esquerda
        if (fatorDeBalanceamento(raiz->direito) > 0)
            raiz->direito = rotacaoDireita(raiz->direito);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}
//novo registro na arvore
No* inserir(No* raiz, Registro reg) {
    if (!raiz) {
        No* novo = new No{reg, 0, nullptr, nullptr};
        return novo;//novo nó se tiver vazia
    }
    if (reg.instant < raiz->reg.instant)
        raiz->esquerdo = inserir(raiz->esquerdo, reg);
    else if (reg.instant > raiz->reg.instant)
        raiz->direito = inserir(raiz->direito, reg);
    else
        return raiz;
//atualiza altura
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;
    return balancear(raiz);
}
//inserirmaximo
const int ALTURA_MAXIMA = 10; // Defina a altura máxima desejada

No* inserirmax(No* raiz, Registro reg) {
    if (alturaDoNo(raiz) >= ALTURA_MAXIMA) {
        std::cout << "Árvore atingiu a altura máxima. Não é possível inserir mais nós.\n";
        return raiz;
    }
    
    // Restante da função permanece igual
    // ...
}

No* buscar(No* raiz, int key) {
    No* currentNode = raiz;    //currentNode é um ponteiro auxiliar

    while (currentNode != nullptr) { //verifica até não ter mais nada= nullptr 
        if (key == currentNode->reg.instant) {
            // Dados encontrados: exibe informações detalhadas
            printf("\n[DADO ENCONTRADO]\n");
            printf("---------------------------------\n");
            printf("Buscando o dado: %d\n", currentNode->reg.instant);//verifica instant
            printf("Data (dteday): %s\n", currentNode->reg.dteday.c_str());//Mostra a data associada ao registro
            printf("Hora (hr): %d\n", currentNode->reg.hr);//Exibe a hora do registro
            printf("Temperatura (temp): %.2f°C\n", currentNode->reg.temp);//Mostra a temperatura com precisão
            printf("Umidade (hum): %.2f%%\n", currentNode->reg.hum);//Exibe a umidade em porcentagem
            printf("Velocidade do Vento (windspeed): %.2f km/h\n", currentNode->reg.windspeed);//Mostra a velocidade do vento com unidade
            printf("Tipo de Clima (weathersit): %d\n", currentNode->reg.weathersit);//Indica o código do clima
            printf("---------------------------------\n");
            return currentNode;
            //vanega pela arvore
        } else if (key < currentNode->reg.instant) {
            currentNode = currentNode->esquerdo; // Busca à esquerda
        } else {
            currentNode = currentNode->direito;  // Busca à direita
        }
    }
    
    printf("[ERRO] Dado com chave %d não encontrado na árvore.\n", key);
    return nullptr;//retorna nulo se nao encontrar
}

//percorre arvore em ordem crescente esquerda-> raiz> direit
void emOrdem(No* raiz) {
    if (!raiz) return;
    emOrdem(raiz->esquerdo);//visita esquerda
    std::cout << raiz->reg.instant << " -> " << raiz->reg.cnt << "\n";//exibe raiz
    emOrdem(raiz->direito);//visita direita
}



