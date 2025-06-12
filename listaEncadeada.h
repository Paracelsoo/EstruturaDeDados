#ifndef BICICLETAS_H
#define BICICLETAS_H

//estrutura dos dados por linha dataset por linha 
typedef struct{
	//dados do dataset que queremos conservar 
	char dteday[25]; 
    float atemp, hum, windspeed;
    int mnth, holiday, casual, weekday, registered;
	int cnt, workingday, weathersit, season, yr, hr;
}bicicletasHora;

//estrutura lista encadeada 
//estrutura do no da lista encadeada
typedef struct No {
    bicicletasHora dados;
    struct No* proximo;
} No;

// estrutura para definir o que esta sendo inserido na lista encadeada
typedef struct{
 	No* inicio;
    No* fim;
    int tamanho;
}listaBicicletas;

// Protótipos das funções
No* criarNo(bicicletasHora valor);
No* buscarNoFiltro(listaBicicletas* lista, char* date, int hr);

int lerArquivoCSV(listaBicicletas* lista);

void limparLista(listaBicicletas* lista);
void inicializarLista(listaBicicletas* lista);
void inserirInicio(listaBicicletas* lista, bicicletasHora dados);
void inserirFinal(listaBicicletas* lista, bicicletasHora dados);
void removerInicio(listaBicicletas* lista);
void removerFim(listaBicicletas* lista);
void removerMeio(listaBicicletas* lista, char* date, int hr);
void printDadosDeAte(listaBicicletas* lista, int i, int parada);
void printNo(No* no);



#endif