
// ==== FILE: csv_reader.cpp ====
#include "csv_reader.h"//Contém a definição da estrutura Registro e do protótipo da função lerCSV.
#include <fstream> //Para operações de leitura de arquivos (ifstream).
#include <sstream> // Para manipulação de strings como streams (stringstream).
#include <iostream>//Para mensagens de erro (cerr).

std::vector<Registro> lerCSV(const std::string& caminho) {
//Retorna um vetor de Registro (todos os dados lidos do CSV). Recebe o caminho do arquivo (caminho) como referência constante.
    std::ifstream file(caminho);
    std::string linha;// Armazena cada linha do CSV temporariamente.
    std::vector<Registro> dados;// Vetor que acumulará os registros lidos.

    if (!file.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir o arquivo: " << caminho << "\n";
        return dados;
    }

    getline(file, linha); // cabeçalho: lê a primeira linha: nomes de colunas

    while (getline(file, linha)) {
        std::stringstream ss(linha); //Converte a linha em um stream para extrair os campos.
        Registro r;// Variável temporária para armazenar um registro.
        std::string item;// Armazena cada campo lido antes da conversão.
        getline(ss, item, ','); r.instant = stoi(item);
		// lê um campo até a próxima vírgula (delimitador).
        //stoi(): Converte string para int.
        //stof(): Converte string para float.
		getline(ss, r.dteday, ',');
        getline(ss, item, ','); r.season = stoi(item);
        getline(ss, item, ','); r.yr = stoi(item);
        getline(ss, item, ','); r.mnth = stoi(item);
        getline(ss, item, ','); r.hr = stoi(item);
        getline(ss, item, ','); r.holiday = stoi(item);
        getline(ss, item, ','); r.weekday = stoi(item);
        getline(ss, item, ','); r.workingday = stoi(item);
        getline(ss, item, ','); r.weathersit = stoi(item);
        getline(ss, item, ','); r.temp = stof(item);
        getline(ss, item, ','); r.atemp = stof(item);
        getline(ss, item, ','); r.hum = stof(item);
        getline(ss, item, ','); r.windspeed = stof(item);
        getline(ss, item, ','); r.casual = stoi(item);
        getline(ss, item, ','); r.registered = stoi(item);
        getline(ss, item, ','); r.cnt = stoi(item);
        dados.push_back(r);//Adiciona o registro r ao vetor dados após preencher todos os campos.
    }

    return dados;
}



