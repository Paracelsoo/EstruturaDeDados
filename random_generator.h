#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include "csv_reader.h" // Já contém a definição de Registro
#include <vector>

Registro criarRegistroAleatorio();
std::vector<Registro> criarRegistrosAleatorios(int quantidade);
int gerar_numero_aleatorio();
#endif