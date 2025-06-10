#include <iostream>
#include <chrono>

using namespace std;

short alturaDoNo(No* no);
short maior(short a, short b);
No* rotacaoEsquerda(No* r);
No* rotacaoDireita(No* r);
No* balancear(No* raiz);  // já deve existir em seu código

// Encontra o nó com o menor valor (usado para substituição)
No* minimoNo(No* raiz) {
    No* atual = raiz;
    while (atual && atual->esquerdo != NULL)
        atual = atual->esquerdo;
    return atual;
}

No* remover(No* raiz, int instant) {
   
    cout << "[REMOVER] Tentando remover valor: " << instant << "\n";

    if (!raiz) {
        cout << "[REMOVER] Valor não encontrado.\n";
        return NULL;
    }

    if (instant < raiz->reg.instant)
        raiz->esquerdo = remover(raiz->esquerdo, instant);
    else if (instant > raiz->reg.instant)
        raiz->direito = remover(raiz->direito, instant);
    else {
        // Caso 1: Nó com um ou nenhum filho
        if (!raiz->esquerdo || !raiz->direito) {
            No* temp = raiz->esquerdo ? raiz->esquerdo : raiz->direito;
            if (!temp) {
                // Nenhum filho
                temp = raiz;
                raiz = NULL;
            } else {
                // Um filho
                *raiz = *temp;
            }
            free(temp);
            cout << "\n\n[REMOVER] Nó"<<instant<<" removido.\n\n";
        } else {
            // Caso 2: Dois filhos — pegar o menor da subárvore direita
            No* temp = minimoNo(raiz->direito);
            raiz->reg = temp->reg;
            raiz->direito = remover(raiz->direito, temp->reg.instant);
        }
    }

    if (!raiz) return raiz; // Se a árvore ficou vazia

    // Recalcula altura e balanceia
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;
    raiz = balancear(raiz);

    return raiz;
}


